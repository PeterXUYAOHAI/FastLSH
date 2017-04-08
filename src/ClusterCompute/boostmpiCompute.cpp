/***
Copyright 2017 Yaohai XU

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***/

/**
    FastLSH
    boostmpiCompute.cpp
    Purpose: This is the source file for FastLSH-MPI

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

/**
 * Execution command
 * mpiexec -np 4 -host prj67,prj68,prj69,prj70 -mca btl ^openib ./boostmpiLSH

 * boost install command
 * sudo apt-get install libboost-all-dev
 * example cmd to run--
 * mpirun -np 10 --hosts slave1 ./boostmpiCompute

 * compliaiton cmd
 * mpic++ -std=c++11 -L/usr/lib -I/usr/include/boost/mpi -lboost_filesystem -lboost_system -lboost_mpi boostmpiCompute.cpp

 * send multi dimensional vector needs serialization, I will flaten the multidimwnsional vector and send them first, it
 * might drag down the speed.
 * this program stop at when the collision table finishs calculation on each process, but not gather in the mater node.

 * mpiexec -np 2 -host --allow-run-as-root -mca btl ^openib ./boostmpiLSH
 * mpiexec -np 8 --allow-run-as-root -host -mca btl ^openib ./boostmpiLSH
 */


#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>

namespace mpi = boost::mpi;

typedef std::vector<std::vector<std::vector<double> > > vector3D;
typedef std::vector<std::vector<double> > vector2D;
typedef std::vector<double> vector1D;

#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

/**
 * load file from Linux File System
 * @param filePath
 * @param row number of rows
 * @param col number of columns
 * @return 2D vector<double> dataset from csv
 */
vector2D loadDataFromLinuxSystem(char* filePath, size_t row, size_t col);

/**
 * flat a 2D vector into 1D vector
 * @param origVec the original 2D vector
 * @return 1D vector<double> of flatted vector
 */
vector1D flat2D(vector2D origVec);

/**
 * flat a 2D vector of candidate set (different length of each row)
 * @param origCandidate original candidate set
 * @return 1D vector<double> of flatted vector
 */
vector1D flat2DCandidate(vector2D origCandidate);

/**
 * flat a 3D vector into 1D vector
 * @param origVec original 3D vector
 * @return 1D vector of the flatten vector
 */
vector1D flat3D(vector3D origVec);

/**
 * reconstruct 1D vector to 2D vector
 * @param origVec the 1D VECTOR
 * @param d1 the length of first dimension
 * @param d2 the length of second dimension
 * @return 2D vector<double> of recontructed 2D vector
 */
vector2D reconstr2D(vector1D origVec, int d1, int d2);

/**
 * reconstruct 2D vector of candidate set (different length of each row)
 * @param candidateRecv the 1D vector
 * @param lineNum the number of lines
 * @return 2D reconstructed vector<double>
 */
vector2D reconstr2DCandidate(vector1D candidateRecv, int lineNum);

/**
 * reconstruct 3D vector
 * @param origVec the 3D vector
 * @param d1 number of index of first dimension
 * @param d2 number of index of second dimension
 * @param d3 number of index of third dimension
 * @return 3D vector<double>
 */
vector3D reconstr3D(vector1D origVec, int d1, int d2, int d3);

/**
 * compute the hash value
 * @param dataset the original dataset
 * @param randomLine random Lines generated
 * @param randomVector random numbers generated
 * @param pNum number of points
 * @param L # of group hash
 * @param K # the number of hash functions in each group hash
 * @param D # of dimensions
 * @param W bucket width
 * @return the 2D vector hash value
 */
vector2D computeHash(vector2D dataset, vector3D randomLine, vector1D randomVector, size_t pNum, size_t L, size_t K,
                     size_t D, size_t W);

/**
 * compute the collision matrix
 * @param hMatrixN 2D hash matrix N
 * @param hMatrixQ 2D hash matrix Q
 * @param Q # of vectors in the queryset
 * @param N # of vectors in the dataset
 * @param L # of group hash
 * @return 2D vector collision table
 */
vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ, size_t Q, size_t N, size_t L);

/**
 * max-min normalization
 * @param dataset original dataset
 * @return 2D vector of normalized dataset
 */
vector2D normalize(vector2D dataset);

/**
 * compute the candidate set in normal style
 * @param Q # of vectors in the queryset
 * @param N # of vectors in the dataset
 * @param T threshold
 * @param collisionMatrix the collision matrixes
 * @return 2D vector<double> of candidate set
 */
vector2D computeCandidateNormal(size_t Q, size_t N, size_t T, vector2D collisionMatrix);

int main (int argc, char **argv) {

    //prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::high_resolution_clock::time_point t3;
    std::chrono::high_resolution_clock::time_point t4;

    //start first timing
    t1 = now();

    // initialize the mpi environment
    mpi::environment env;
    mpi::communicator world;

    //print identity to the screen
    std::cout<<"My name is "<<env.processor_name()<<" Processor id "<<world.rank()<<"\n";

    //parameter sets
    size_t N = 1000000; //# of vectors in the dataset
    size_t Q = 100; //# of vertors in the queryset
    size_t D = 57; //# of dimensions
    size_t L = 200; //# of group hash
    size_t K = 1; //# the number of hash functions in each group hash
    double W = 1.2; //bucket width
    size_t  T = 100; // threshold
    int root_process = 0;
    char * inputPathSetN = "/home/mpiuser/cloud/dataset4259934NoIndex.csv" ;
    char * inputPathSetQ = "/home/mpiuser/cloud/dataset100NoIndex.csv";
    char * outputPath;

    //1d structure for communication between process
    vector1D setQ1d;
    vector1D randomLine1d;
    vector1D partialSetN1d;

    //variables every process own
    vector1D UniformRandomVector;
    vector3D randomLine;
    vector2D setQ;
    vector2D setQNorm;
    vector2D hashValueQ;

    //the splited part of N for each thread
    vector2D partialSetN;
    vector2D partialHashValueN;
    vector2D partialCollisionTable;
    vector2D partialCandidateSet;

    //as the name indicates
    int numNperSlave;
    numNperSlave = ceil(N/(double)world.size());

    std::cout<<"start to calculate random \n";
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
        setQ = loadDataFromLinuxSystem(inputPathSetQ, Q, D);

        setQNorm = normalize(setQ);
        //flatten setQ for transmission
        setQ1d = flat2D(setQNorm);
        //flatten randomLine for transmission
        randomLine1d = flat3D(randomLine);
    }

    //broadcast the shared variables
    broadcast(world, setQ1d, 0);
    broadcast(world, randomLine1d, 0);
    broadcast(world, UniformRandomVector, 0);


    //let slaves reconstruct the multi-dimensional vector
    if(world.rank()!=root_process) {
        randomLine = reconstr3D(randomLine1d, L, K, D);
        setQNorm = reconstr2D(setQ1d,Q,D);
    }

    //deal with N
    if (world.rank()==root_process){
        vector2D setN;
        vector2D setNNorm;
        setN = loadDataFromLinuxSystem(inputPathSetN, N, D);
        setNNorm = normalize(setN);
        {
            vector2D temp;
            setN.swap(temp);
        }
        //deal with sending splited parts of N
        {

            //the last part of N will be done by master node(it contains least num of N)
            for (int i = 0; i <world.size() ; ++i) {

                if(i!=(world.size()-1)) {
                    vector2D::const_iterator first = setNNorm.begin() +  numNperSlave * i;
                    vector2D::const_iterator last = setNNorm.begin() +  numNperSlave * (i + 1);
                    vector2D newVec(first, last);

                    vector1D vecToSend = flat2D(newVec);
                    world.send(i + 1, 0, vecToSend);
                }

                    // master node get last part of the job (usually less than other parts
                else{
                    vector2D::const_iterator first = setNNorm.begin() +  numNperSlave * i;
                    vector2D::const_iterator last = setNNorm.begin() + N;
                    vector2D newVec(first, last);
                    partialSetN = newVec;
                }
            }
            {
                vector2D temp;
                setNNorm.swap(temp);
            }       }

        t4=now();
        auto duration2 = dcast( t4 - t3 ).count();
        std::cout <<duration2 << " μ send out N set\n";
    }
        //slave nodes receive N
    else{
        world.recv(0, 0, partialSetN1d);
        partialSetN = reconstr2D(partialSetN1d, numNperSlave, D);
    }


    // calculate the hash and collision (separate master and slave operation because their num of N might be different)
    if(world.rank()==root_process){
        // FastLSH execution flow
        vector2D gatheredCandidateSet;
        hashValueQ = computeHash(setQNorm, randomLine, UniformRandomVector, Q, L, K, D ,W );
        partialHashValueN = computeHash(partialSetN, randomLine, UniformRandomVector,
                                        N- numNperSlave*(world.size()-1), L, K, D, W);
        partialCollisionTable = computeCollision(partialHashValueN,hashValueQ,Q,N- numNperSlave*(world.size()-1),L);
        partialCandidateSet = computeCandidateNormal(Q, N- numNperSlave*(world.size()-1), T, partialCollisionTable);
        for (int i = 1; i < world.size(); ++i) {
            vector1D candidateRecv;
            world.recv(i,0,candidateRecv);
            vector2D temp = reconstr2DCandidate(candidateRecv, Q);
            for (int j = 0; j < temp.size(); ++j) {
                gatheredCandidateSet.push_back(temp[0]);
            }
        }

        for (int k = 0; k < partialCandidateSet.size(); ++k) {
            gatheredCandidateSet.push_back(partialCandidateSet[k]);
        }

    }

    else{
        // FastLSH execution flow
        hashValueQ = computeHash(setQNorm, randomLine, UniformRandomVector, Q, L, K, D ,W );
        partialHashValueN = computeHash(partialSetN, randomLine, UniformRandomVector, numNperSlave, L, K, D, W);
        partialCollisionTable = computeCollision(partialHashValueN,hashValueQ,Q,numNperSlave,L);
        partialCandidateSet = computeCandidateNormal(Q, numNperSlave, T, partialCollisionTable);
        vector1D candidate1D  = flat2DCandidate(partialCandidateSet);
        world.send(0,0,candidate1D);

    }

    //leave them for testing
//    std::cout<<"v1Process # " << world.rank() <<" "<<partialCollisionTable[0][0]<<"\n";
//    std::cout<<"v1Process # " << world.rank() <<" "<<partialCollisionTable[0][4]<<"\n";
//    std::cout<<"v2Process # " << world.rank() <<" "<<partialCollisionTable[0][5]<<"\n";

    t2 = now();
    auto duration = dcast( t2 - t1 ).count();
    std::cout <<"processor "<<world.rank()<< " uses "<<duration << " μs to compute\n";


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
    //parse csv file
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

vector1D flat2DCandidate(vector2D origCandidate){
    vector1D result;
    for (int i = 0; i < origCandidate.size(); ++i) {

        for (int j = 0; j < origCandidate[0].size(); ++j) {
            result.push_back(origCandidate[i][j]);
        }
        //-1 as flag to indicate new line
        result.push_back(-1);
    }
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


vector2D reconstr2DCandidate(vector1D candidateRecv, int lineNum){
    vector2D result;
    for (int i = 0; i < lineNum; ++i) {
        vector1D vL(0,0);
        result.push_back(vL);
    }
    int line = 0;
    for (int i = 0; i < candidateRecv.size(); ++i) {
        if(candidateRecv[i]==-1) {
            line++;
            continue;
        }
        result[line].push_back(candidateRecv[i]);
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

    for (int i = 0; i < pNum; ++i) {
        vector1D vL(L,0);
        hashMatrix.push_back(vL);
    }

    //loop through # of data point
#pragma omp parallel for collapse(2)
    for (int n = 0; n < pNum; ++n) {
        //loop through # of hash function group
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
            hashMatrix[n][l]=hashFinalValue;
        }

    }
    return hashMatrix;
}


vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ, size_t Q, size_t N, size_t L){
    vector2D collisionMatrix;

    for (int i = 0; i < Q ; ++i) {
        vector1D singleLine(N, 0);
        collisionMatrix.push_back(singleLine);
    }

#pragma omp parallel for collapse(2)
    for (int q = 0; q <Q ; ++q) {
        for (int n = 0; n <N ; ++n){
            for (int hash_id = 0; hash_id < L; ++hash_id) {
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
                    collisionMatrix[q][n]++;
            }
        }
    }
    return collisionMatrix;
}

vector2D normalize(vector2D dataset){
    size_t row = dataset.size();
    size_t col = dataset[0].size();

    vector1D maximums(col, -999999);
    vector1D minimums(col, 9999999);

    //find maximums and minimums of each column
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(dataset[i][j]>= maximums[j])
                maximums[j]=dataset[i][j];
            if(dataset[i][j]<=minimums[j])
                minimums[j]=dataset[i][j];
        }
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            //if max equals min, the value is set to 0.5
            if(maximums[j]==minimums[j])
                dataset[i][j] = 0.5;
            else
                //normalization formula -- (x-min(x))/(max(x)-min(x))
                dataset[i][j] = (dataset[i][j] - minimums[j])/(maximums[j]-minimums[j]);
        }
    }
    return dataset;
}


vector2D computeCandidateNormal(size_t Q, size_t N, size_t T, vector2D collisionMatrix){
    vector2D candidateSet;
    for (int i = 0; i < Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

#pragma omp parallel for
    for (int i = 0; i < Q; ++i) {
        vector1D candidates;
        for (int j = 0; j < N; ++j) {
            if(collisionMatrix[i][j]>=T)
                candidates.push_back((double &&) j);
        }
        candidateSet[i] = candidates;
    }
    return candidateSet;
}