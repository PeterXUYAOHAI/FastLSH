#include <cmath>
#include <thread>
#include <algorithm>
#include "../../include/otherClasses/Computer.h"

vector2D ComputerSingleThread::computeHash(vector2D dataset, size_t pNum){
    vector2D hashMatrix;


    //loop through # of data point
    for (int n = 0; n < pNum; ++n) {
        //loop through # of hash function group
        vector1D vL;

        for (int l = 0; l < ph.L; ++l) {
            double hashFinalValue = 0;
            //loop through the inner of a hash function group
            for (int k = 0; k < ph.K; ++k) {
                double dTemp = 0;
                //loop through all the dimensions
                for (int d = 0; d < ph.D; ++d) {
//                    std::cout<<ph->K<<"\n";
                    //vector(math) multiply to make projection
                    dTemp += (ph.randomLine)[l][k][d]*dataset[n][d];

                }
                //assign hashvalue into bucket
                double hashvalue = floor(dTemp/ph.W);
                //merge hash group results **see documentation
                hashFinalValue = hashvalue*ph.randomVector[k] + hashFinalValue;
            }
            vL.push_back(hashFinalValue);
        }
        hashMatrix.push_back(vL);

    }
    return hashMatrix;
}

vector2D ComputerSingleThreadNormal::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D collisionMatrix;
    for (int q = 0; q <ph.Q ; ++q) {
        vector1D singleLine(ph.N, 0);
        for (int n = 0; n <ph.N ; ++n){
            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
                //cast to int for comparision, reduce double compare risk
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
                    singleLine[n]++;
            }
        }
        collisionMatrix.push_back(singleLine);
    }
    return collisionMatrix;
}



vector2D ComputerSingleThreadNormal::computeCandidate(vector2D collisionMatrix){
    vector2D candidateSet;
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

    for (int i = 0; i < ph.Q; ++i) {
        vector1D candidates;
        for (int j = 0; j < ph.N; ++j) {
            if(collisionMatrix[i][j]>=ph.T)
                candidates.push_back((double &&) j);
        }
        candidateSet[i] = candidates;
    }
    return candidateSet;
}


//by quick mode, it means get candidateset directly, skip the collision table
vector2D ComputerSingleThreadQuick::computeCandidate(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D candidateSet;
    //Prepare set to store candidates
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }
    for (int q = 0; q <ph.Q; ++q) {
        vector1D singleRow(0, 0);
        for (int n = 0; n <ph.N ; ++n){
            int colliNum = 0;
            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id]) {
                    colliNum++;
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
