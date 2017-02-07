//
// Created by peter on 17-2-7.
//

#include "../include/LSH.h"
#include <random>


vector2D LSH::computeHash_openmp(vector2D dataset, size_t pointNum){
    vector2D hashMatrix;

    for (int i = 0; i < pointNum; ++i) {
        vector1D vL(L,0);
        hashMatrix.push_back(vL);
    }

    //loop through # of data point
    #pragma omp parallel for collapse(2)
    for (int n = 0; n < pointNum; ++n) {
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
            hashMatrix[n][l] = hashFinalValue;
        }
    }
    return hashMatrix;
}

vector2D LSH::computeCollision_openmp(vector2D hMatrixN, vector2D hMatrixQ){
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