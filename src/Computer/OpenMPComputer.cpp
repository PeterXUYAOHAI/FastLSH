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
    OpenMPComputer.cpp
    Purpose: This is the source file for OpenMP computing functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <cmath>
#include <thread>
#include <algorithm>
#include <iostream>
#include "../../include/otherClasses/Computer.h"

vector2D ComputerOpenMP::computeHash(vector2D dataset, size_t pointNum){
    vector2D hashMatrix;
    //pre-allocate memory space
    for (int i = 0; i < pointNum; ++i) {
        vector1D vL(ph.L,0);
        hashMatrix.push_back(vL);
    }

    //loop through # of data point
#pragma omp parallel for collapse(2)
    for (int n = 0; n < pointNum; ++n) {
        //loop through # of hash function group
        for (int l = 0; l < ph.L; ++l) {
            double hashFinalValue = 0;
            //loop through the inner of a hash function group
            for (int k = 0; k < ph.K; ++k) {
                double dTemp = 0;
                //loop through all the dimensions
                for (int d = 0; d < ph.D; ++d) {
                    //vector(math) multiply to make projection
                    dTemp += dataset[n][d]*ph.randomLine[l][k][d];
                }
                //assign hashvalue into bucket
                double hashvalue = floor(dTemp/ph.W);
                //merge hash group results **see documentation
                hashFinalValue = hashvalue*ph.randomVector[k] + hashFinalValue;
            }
            hashMatrix[n][l] = hashFinalValue;
        }
    }
    return hashMatrix;
}


vector2D ComputerOpenMPNormal::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
    //pre-allocate memory
    vector2D collisionMatrix;
    for (int i = 0; i < ph.Q ; ++i) {
        vector1D singleLine(ph.N, 0);
        collisionMatrix.push_back(singleLine);
    }

#pragma omp parallel for collapse(2)
    for (int q = 0; q <ph.Q ; ++q) {
        for (int n = 0; n <ph.N ; ++n){
            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
                // if N , Q have same hash value, increment the collision number
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
                    collisionMatrix[q][n]++;
            }
        }
    }
    return collisionMatrix;
}

vector2D ComputerOpenMPNormal::computeCandidate(vector2D collisionMatrix){
    //pre-allocate memory
    vector2D candidateSet;
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

#pragma omp parallel for
    for (int i = 0; i < ph.Q; ++i) {
        vector1D candidates;
        for (int j = 0; j < ph.N; ++j) {
            //if the collision count is higher than T, push the candidate
            if(collisionMatrix[i][j]>=ph.T)
                candidates.push_back((double &&) j);
        }
        candidateSet[i] = candidates;
    }

    return candidateSet;
}


//by quick mode, it means get candidateset directly, skip the collision table
vector2D ComputerOpenMPQuick::computeCandidate(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D candidateSet;
    //Preallocate the memory
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

#pragma omp parallel for
    for (int q = 0; q <ph.Q ; ++q) {
        vector1D singleRow(0, 0);
        for (int n = 0; n <ph.N ; ++n){
            int colliNum = 0;
            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id]) {
                    colliNum++;
                    //once the count is larger than threshold, push back the candidate and break the loop
                    if(colliNum>=ph.T){
                        singleRow.push_back((double &&) n);
                        break;
                    }
                }
            }
        }
        candidateSet[q] = singleRow;
    }

    return candidateSet;
}

void ComputerOpenMP::printThreadMode(){
    std::cout<< "OpenMP";
}
void ComputerOpenMPNormal::printComputeMode(){
    std::cout<< "Normal";
}
void ComputerOpenMPQuick::printComputeMode(){
    std::cout<< "Quick";
}
