////
//// Created by peter on 17-2-7.
////
//
//#include "../../include/LSH.h"
//
//#include <random>
//
//
//
//
//vector2D LSH::computeHash(vector2D dataset, size_t pNum){
//    vector2D hashMatrix;
//
//
//    //loop through # of data point
//    for (int n = 0; n < pNum; ++n) {
//        //loop through # of hash function group
//        vector1D vL;
//        for (int l = 0; l < ph.L; ++l) {
//            double hashFinalValue = 0;
//            //loop through the inner of a hash function group
//            for (int k = 0; k < ph.K; ++k) {
//                double dTemp = 0;
//                //loop through all the dimensions
//                for (int d = 0; d < ph.D; ++d) {
//                    //vector(math) multiply to make projection
//                    dTemp += dataset[n][d]*ph.randomLine[l][k][d];
//                }
//                //assign hashvalue into bucket
//                double hashvalue = floor(dTemp/ph.W);
//                //merge hash group results **see documentation
//                hashFinalValue = hashvalue*ph.randomVector[k] + hashFinalValue;
//            }
//            vL.push_back(hashFinalValue);
//        }
//        hashMatrix.push_back(vL);
//    }
//    return hashMatrix;
//}
//
//vector2D LSH::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
//    vector2D collisionMatrix;
//    for (int q = 0; q <ph.Q ; ++q) {
//        vector1D singleLine(ph.N, 0);
//        for (int n = 0; n <ph.N ; ++n){
//            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
//                //cast to int for comparision, reduce double compare risk
//                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
//                    singleLine[n]++;
//            }
//        }
//        collisionMatrix.push_back(singleLine);
//    }
//    return collisionMatrix;
//}
//
//
//
//vector2D LSH::computeCandidateNormal(){
//    vector2D candidateSet;
//    for (int i = 0; i < ph.Q; ++i) {
//        vector1D temp(0,0);
//        candidateSet.push_back(temp);
//    }
//
//    for (int i = 0; i < ph.Q; ++i) {
//        vector1D candidates;
//        for (int j = 0; j < ph.N; ++j) {
//            if(collisionMatrix[i][j]>=ph.T)
//                candidates.push_back((double &&) j);
//        }
//        candidateSet[i] = candidates;
//    }
//    return candidateSet;
//}
//
//
////by quick mode, it means get candidateset directly, skip the collision table
//vector2D LSH::computeCandidatesQuick(vector2D hMatrixN, vector2D hMatrixQ, size_t T){
//    vector2D candidateSet;
//    //Prepare set to store candidates
//    for (int i = 0; i < ph.Q; ++i) {
//        vector1D temp(0,0);
//        candidateSet.push_back(temp);
//    }
//    for (int q = 0; q <ph.Q ; ++q) {
//        vector1D singleRow(0, 0);
//        for (int n = 0; n <ph.N ; ++n){
//            int colliNum = 0;
//            for (int hash_id = 0; hash_id < ph.L; ++hash_id) {
//                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id]) {
//                    colliNum++;
//                    if(colliNum>=T){
//                        singleRow.push_back((double &&) n);
//                        break;
//                    }
//
//                }
//            }
//        }
//        candidateSet[q] = singleRow;
//    }
//
//    return candidateSet;
//}