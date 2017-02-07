//
// Created by peter on 17-2-7.
//

#include "../include/LSH.h"
#include <random>

vector2D LSH::computeHash(vector2D dataset, size_t pNum){
    vector2D hashMatrix;
//    vector3D randomLine = generateRandomLine(L,K,D);
//    vector1D randomVector = generateUniformRandomVector(K,W);

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
