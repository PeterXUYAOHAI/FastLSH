//
// Created by peter on 17-2-17.
//

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>

namespace mpi = boost::mpi;

typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;


// example cmd to run-- mpirun -np 10 --hosts slave1 ./boostmpiCompute


/*** send multi dimensional vector needs serialization, I will flaten the multidimwnsional vector and send them first, it
 might drag down the speed.

 this program stop at when the collision table finishs calculation on each process, but not gather in the mater node.
 ***/
vector2D loadDataFromLinuxSystem(char* filePath, size_t row, size_t col);

vector1D flat2D(vector2D origVec);

vector1D flat3D(vector3D origVec);

vector2D reconstr2D(vector1D origVec, int d1, int d2);

vector3D reconstr3D(vector1D origVec, int d1, int d2, int d3);

vector2D computeHash(vector2D dataset, vector3D randomLine, vector1D randomVector, size_t pNum, size_t L, size_t K,
                     size_t D, size_t W);

vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ, size_t Q, size_t N, size_t L);


int main (int argc, char **argv) {

    // initialize the mpi environment
    mpi::environment env;
    mpi::communicator world;

    //hardcode the paremeters
    size_t N = 1000; //# of vectors in the dataset
    size_t Q = 1000; //# of vertors in the queryset
    size_t D = 57; //# of dimensions
    size_t L = 200; //# of group hash
    size_t K = 1; //# the number of hash functions in each group hash
    double W = 1.2; //bucket width
    int root_process = 0;

    //1d structure for communication between process
    vector1D setQ1d;
    vector1D randomLine1d;
    vector1D partialSetN1d;

    //variables every process own
    vector1D UniformRandomVector;
    vector3D randomLine;
    vector2D setQ;
    vector2D hashValueQ;

    //the part of N for each thread
    vector2D partialSetN;
    vector2D partialHashValueN;
    vector2D partialCollisionTable;

    //as the name indicates
    int numNperSlave;
    numNperSlave = ceil(N/(double)world.size());


    //the master node do the preparation
    if(world.rank() == root_process) {
        //generate random line
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i < L; i++) {
            vector2D vK;
            for (int j = 0; j < K; ++j) {
                //generate random according normal distribution
                std::normal_distribution<double> distribution(0.5, 0.5);
                std::vector<double> vD;
                for (int k = 0; k < D; ++k) {
                    vD.push_back(distribution(gen));
                }
                vK.push_back(vD);
            }
            randomLine.push_back(vK);
        }

        //generate random vector
        std::uniform_real_distribution<> dis(0, 1.2);
        for (int i = 0; i < 1; ++i) {
            UniformRandomVector.push_back(dis(gen));
        }

        //read in setQ
        setQ = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", Q, D);
        //flatten setQ for transmission
        setQ1d = flat2D(setQ);
        //flatten randomLine for transmission
        randomLine1d = flat3D(randomLine);
    }

    //broadcast the shared variables
    broadcast(world, setQ1d, 0);
    broadcast(world, randomLine1d, 0);
    broadcast(world, UniformRandomVector, 0);


    //let slaves reconstruct the multi dimensional vector
    if(world.rank()!=root_process) {
        randomLine = reconstr3D(randomLine1d, L, K, D);
        setQ = reconstr2D(setQ1d,Q,D);
    }

    //deal with N
    if (world.rank()==root_process){
        vector2D setN;
        setN = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", N, D);

        //deal with send part of N
        {

            //the last part of N will be done by master node(it contains least num of N)
            for (int i = 0; i <world.size() ; ++i) {

                if(i!=(world.size()-1)) {
                    vector2D::const_iterator first = setN.begin() +  numNperSlave * i;
                    vector2D::const_iterator last = setN.begin() +  numNperSlave * (i + 1);
                    vector2D newVec(first, last);

                    vector1D vecToSend = flat2D(newVec);
                    world.send(i + 1, 0, vecToSend);
                }

                    // master node get last part of the job
                else{
                    vector2D::const_iterator first = setN.begin() +  numNperSlave * i;
                    vector2D::const_iterator last = setN.begin() + N;
                    vector2D newVec(first, last);
                    partialSetN = newVec;
                }
            }
        }
    }
    //slave nodes receive N
    else{
        world.recv(0, 0, partialSetN1d);
        partialSetN = reconstr2D(partialSetN1d, numNperSlave, D);
    }


    // calculate the hash and collision (separate master and slave operation because their num of N might be different
    if(world.rank()==root_process){
        hashValueQ = computeHash(setQ, randomLine, UniformRandomVector, Q, L, K, D ,W );
        partialHashValueN = computeHash(partialSetN, randomLine, UniformRandomVector,
                                        N- numNperSlave*(world.size()-1), L, K, D, W);
        partialCollisionTable = computeCollision(partialHashValueN,hashValueQ,Q,N- numNperSlave*(world.size()-1),L);

    }

    else{
        hashValueQ = computeHash(setQ, randomLine, UniformRandomVector, Q, L, K, D ,W );
        partialHashValueN = computeHash(partialSetN, randomLine, UniformRandomVector, numNperSlave, L, K, D, W);
        partialCollisionTable = computeCollision(partialHashValueN,hashValueQ,Q,numNperSlave,L);
    }

    //leave them for testing
//    std::cout<<"v1Process # " << world.rank() <<" "<<partialCollisionTable[0][4]<<"\n";
//    std::cout<<"v2Process # " << world.rank() <<" "<<partialCollisionTable[0][5]<<"\n";


    // use this to pause in IDE
    int c;
    std::cin>>c;

}


vector2D loadDataFromLinuxSystem(char* filePath, size_t row, size_t col) {
    std::ifstream file;// declare file stream:
    file.open(filePath);
    vector2D data;
    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }
    std::string value;
    std::string num;
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(file, value, ',');
            else
                getline(file,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    file.close();
    return data;
}


vector1D flat2D(vector2D origVec){
    vector1D result;
    for (int i = 0; i < origVec.size(); ++i) {
        for (int j = 0; j < origVec[0].size(); ++j) {
            result.push_back(origVec[i][j]);
        }
    }
    return result;
}

vector1D flat3D(vector3D origVec){
    vector1D result;
    for (int i = 0; i < origVec.size(); ++i) {
        for (int j = 0; j < origVec[0].size(); ++j) {
            for (int k = 0; k < origVec[0][0].size(); ++k) {
                result.push_back(origVec[i][j][k]);
            }

        }
    }
    return result;
}


vector2D reconstr2D(vector1D origVec, int d1, int d2){
    vector2D result;
    for (int i = 0; i < d1; ++i) {
        vector1D vL(d2,0);
        result.push_back(vL);
    }

    for (int i = 0; i < origVec.size(); ++i) {
        result[i/d2][i%d2] = origVec[i];
    }
    return result;
}

vector3D reconstr3D(vector1D origVec, int d1, int d2, int d3){
    vector3D result;
    for (int i = 0; i < d1; ++i) {
        vector2D vL;
        for (int j = 0; j < d2; ++j) {
            vector1D vD(d3,0);
            vL.push_back(vD);
        }
        result.push_back(vL);
    }

    int interV = d2*d3;

    for (int i = 0; i < origVec.size(); ++i) {
        result[i/interV][(i%(interV))/d3][(i%(interV))%d3] = origVec[i];
    }

    return result;
}



vector2D computeHash(vector2D dataset, vector3D randomLine, vector1D randomVector, size_t pNum, size_t L, size_t K,
                     size_t D, size_t W){
    vector2D hashMatrix;

    //loop through # of data point
    for (int n = 0; n < pNum; ++n) {
        //loop through # of hash function group
        vector1D vL;
        for (int l = 0; l < L; ++l) {
            double hashFinalValue = 0;
            //loop through the inner of a hash function group
            for (int k = 0; k < K; ++k) {
                double dTemp = 0;
                //loop through all the dimensions
                for (int d = 0; d < D; ++d) {
                    //vector(math) multiply to make projection
                    dTemp += dataset[n][d]*randomLine[l][k][d];
                }
                //assign hashvalue into bucket
                double hashvalue = floor(dTemp/W);
                //merge hash group results **see documentation
                hashFinalValue = hashvalue*randomVector[k] + hashFinalValue;
            }
            vL.push_back(hashFinalValue);
        }
        hashMatrix.push_back(vL);
    }
    return hashMatrix;
}


vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ, size_t Q, size_t N, size_t L){
    vector2D collisionMatrix;
    for (int q = 0; q <Q ; ++q) {
        vector1D singleLine(N, 0);
        for (int n = 0; n <N ; ++n){
            for (int hash_id = 0; hash_id < L; ++hash_id) {
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
                    singleLine[n]++;
            }
        }
        collisionMatrix.push_back(singleLine);
    }
    return collisionMatrix;
}

