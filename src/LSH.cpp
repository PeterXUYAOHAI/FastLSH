//
// Created by peter on 17-2-4.
//
#include "../include/LSH.h"
#include <fstream>
#include <iostream>
#include <random>

LSH::LSH() {
}

LSH::LSH(size_t N, size_t D, size_t L, size_t K, double W, size_t Q) {
    this->N = N;
    this->D = D;
    this->L = L;
    this->K = K;
    this->W = W;
    this->Q = Q;
    randomLine = generateRandomLine();
    randomVector = generateUniformRandomVector(K,W);
}


void LSH::loadSetN(std::string filePath){
    setN = loadDataFromLinuxSystem(filePath, N, D);
}

void LSH::loadSetQ(std::string filePath){
    setQ = loadDataFromLinuxSystem(filePath, Q, D);
}

vector2D LSH::loadDataFromLinuxSystem(std::string filePath, size_t row, size_t col) {
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


vector3D LSH::generateRandomLine(){

    std::random_device rd;
    std::mt19937 gen(rd());
    vector3D randomLine;
    for (int i = 0; i < L; i++) {
        vector2D vK;
        for (int j = 0; j < K; ++j) {
            //generate random according normal distribution
            std::normal_distribution<double> distribution(0.5,0.5);
            std::vector<double> vD;
            for (int k = 0; k < D; ++k) {
                vD.push_back(distribution(gen));
            }
            vK.push_back(vD);
        }
        randomLine.push_back(vK);
    }
    return randomLine;
}


vector1D LSH::generateUniformRandomVector(size_t number, double maxium){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, maxium);

    vector1D UniformRandomVector;
    for (int i = 0; i < number; ++i) {
        UniformRandomVector.push_back(dis(gen));
    }

    return UniformRandomVector;
}


vector2D LSH::computeHash(vector2D dataset){
    vector2D hashMatrix;
//    vector3D randomLine = generateRandomLine(L,K,D);
//    vector1D randomVector = generateUniformRandomVector(K,W);

    //loop through # of data point
    for (int n = 0; n < N; ++n) {
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

vector2D LSH::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
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

vector2D LSH::getCollisionMatrix() {

    //get Hash Matrix
    hashMatrixN = computeHash(setN);
    hashMatrixQ = computeHash(setQ);

    //release the memory of the raw sets(setQ, setN), detail see <Effective STL>
    vector2D temp1;
    vector2D temp2;
    setQ.swap(temp1);
    setN.swap(temp2);

    //compute collision matrix
    vector2D collisionMatrix;
    collisionMatrix = computeCollision(hashMatrixN, hashMatrixQ);

//    //release the memory of the hashMatrixs, detail see <Effective STL>
    vector2D temp3;
    vector2D temp4;
    hashMatrixN.swap(temp3);
    hashMatrixQ.swap(temp4);

    return collisionMatrix;
}












