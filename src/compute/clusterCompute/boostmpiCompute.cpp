//
// Created by peter on 17-2-17.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
//
//
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>
#include <mpi.h>
namespace mpi = boost::mpi;

typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

// send multi dimensional vector needs serialization, I will flaten the multidimwnsional vector and send them first, it
// might drag down the speed

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

    std::cout<<interV<<"\n";
    std::cout<<origVec.size()<<"\n";
    for (int i = 0; i < origVec.size(); ++i) {
        std::cout<<i/interV<<"here is good1\n";
        std::cout<<(i%(interV))/d2<<"here is good2\n";
        std::cout<<(i%(interV))%d2<<"here is good3\n";
        result[i/interV][(i%(interV))/d2][(i%(interV))%d2] = origVec[i];

        std::cout<<i<<"here is good\n";
    }

    return result;
}


int main (int argc, char **argv) {

    mpi::environment env;
    mpi::communicator world;

    std::string value;
    if (world.rank() == 0) {
        value = "Hello, World!";
    }

    broadcast(world, value, 0);




    size_t N = 1000; //# of vectors in the dataset
    size_t Q = 1000; //# of vertors in the queryset
    size_t D = 57; //# of dimensions
    size_t L = 200; //# of group hash
    size_t K = 1; //# the number of hash functions in each group hash
    double W = 1.2; //bucket width
    int root_process = 0;


    vector1D setQ1d;
    vector1D randomLine1d;
    vector1D UniformRandomVector;
    vector3D randomLine;
    vector2D setQ;

    vector2D partialSetN;
    vector1D partialSetN1d;
    vector2D hashValueQ;
    vector2D partialHashValueN;
    vector2D partialCollisionTable;
    int numberOfN;



    int numNperSlave;
    numNperSlave = ceil(N/(double)world.size());


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
        std::uniform_real_distribution<> dis(0, 1.2);
        for (int i = 0; i < 1; ++i) {
            UniformRandomVector.push_back(dis(gen));
        }

//        vector2D setN; // original data set of N
//
//        vector2D hashMatrixN; // hashMatrix of N where hash value is stored
//        vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored
        std::cout<<"get 1\n";
        setQ = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", Q, D);
        std::cout<<"get 2\n";
        setQ1d = flat2D(setQ);
        std::cout<<"get 3\n";
        randomLine1d = flat3D(randomLine);
        std::cout<<"get 4\n";
    }

    broadcast(world, setQ1d, 0);
    broadcast(world, randomLine1d, 0);
    broadcast(world, UniformRandomVector, 0);
//
//    std::cout<<"s1Process #" << world.rank() <<randomLine.size();
//    std::cout<<"s2Process #" << world.rank() <<randomLine[0].size();
//    std::cout<<"v1Process #" << world.rank() <<randomLine[0][0][0];
//    std::cout<<"v2Process #" << world.rank() <<randomLine[0][1][1];
//    std::cout<<"s1Process #" <<setQ1d[0]<<std::endl;
//    std::cout<<"s2Process #" <<randomLine1d[1]<<std::endl;








    if(world.rank()!=root_process) {
//        randomLine = reconstr3D(randomLine1d, L, K, D);
        setQ = reconstr2D(setQ1d,Q,D);
    }

//
//    //deal with N
//    if (world.rank()==root_process){
//
//        vector2D setN;
//        setN = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", N, D);
//
//        //deal with send part of N
//        {
//
//            //the last part of N will be done by master node(it contains least num of N)
//            for (int i = 0; i <world.size() ; ++i) {
//
//                if(i!=world.size()) {
//                    vector2D::const_iterator first = setN.begin() + numberOfN * i;
//                    vector2D::const_iterator last = setN.begin() + numberOfN * (i + 1);
//                    vector2D newVec(first, last);
//
//                    vector1D vecToSend = flat2D(newVec);
////                    world.send(i + 1, 0, vecToSend);
//                }
//
//                    // master node get last part of the job
//                else{
//                    vector2D::const_iterator first = setN.begin() + numberOfN * i;
//                    vector2D::const_iterator last = setN.begin() + N;
//                    vector2D newVec(first, last);
//                    partialSetN = newVec;
//                }
//            }
//        }
//    }
//    //slave nodes receive N
//    else{
////        world.recv(0, 0, partialSetN1d);
////        partialSetN = reconstr2D(partialSetN1d, numNperSlave, D);
//    }

    // calculate the hash and collision
    if(world.rank()==root_process){}



    else{

    }

//    std::cout<<"s1Process #" <<world.rank()<<" "<<partialSetN[0][0]<<std::endl;
//    std::cout<<"s2Process #" <<world.rank()<<" "<<partialSetN[0][1]<<std::endl;



         int c;
    std::cin>>c;




}
