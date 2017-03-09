//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include "../../include/Computer.h"


class computerTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
//            mlsh = LSH(300000, 57, 200, 1, 1.2, 1000);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);


//            mlsh.loadSetN("/home/peter/FYP/dataset300000NoIndex.csv", 0);
//            mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;
    Computer *theNormalComputer;
    Computer *theQuickComputer;

};

TEST_F(computerTest, valueTest){


    theNormalComputer = new ComputerSingleThreadNormal(mlsh.N,mlsh.D,mlsh.L,mlsh.K,mlsh.W,mlsh.Q,mlsh.T ,mlsh.randomLine,mlsh.randomVector);
    theQuickComputer = new ComputerSingleThreadQuick(mlsh.N,mlsh.D,mlsh.L,mlsh.K,mlsh.W,mlsh.Q,mlsh.T ,mlsh.randomLine,mlsh.randomVector);

    vector2D hashQ = mlsh.computeHash(mlsh.setQ, mlsh.Q);
    vector2D hashN = mlsh.computeHash(mlsh.setN, mlsh.N);

    vector2D hashQComputer = theNormalComputer->computeHash(mlsh.setQ,mlsh.Q);
    vector2D hashNComputer = theNormalComputer->computeHash(mlsh.setN,mlsh.N);

    vector2D hashNQuickComputer = theQuickComputer->computeHash(mlsh.setN,mlsh.N);

    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theNormalComputer->computeCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);



    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theNormalComputer->computeCandidate(resultOrigin);
    vector2D candidateQuick = theQuickComputer->computeCandidate(mlsh.hashMatrixQ,mlsh.hashMatrixN,mlsh.T);

    std::cout<<"yes";
    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQComputer);
    ASSERT_EQ(hashN, hashNComputer);
    ASSERT_EQ(hashN, hashNQuickComputer);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
    ASSERT_EQ(candidateOrigin, candidateQuick);
}



//TEST_F(computerTest, resultTest) {
//    vector2D singleThreadResult;
//    vector2D openMPResult;
//
//    mlsh.setUseMultiThread(true);
//
//
//    openMPResult = mlsh.getCollisionMatrix();
//    t2 = now();
//
//    auto duration = dcast( t2 - t1 ).count();
//
//    std::cout <<duration << " μs for openMP\n";
//
//
//    mlsh.clearCollisionMatrix();
//
//    mlsh.setUseMultiThread(false);
//
//    t1 = now();
//    singleThreadResult = mlsh.getCollisionMatrix();
//    t2 = now();
//
//    duration = dcast( t2 - t1 ).count();
//
//    std::cout <<duration << " μs for singleThread\n";
//
//    //compare if the two collision counting are same
//    ASSERT_EQ(singleThreadResult.size(), openMPResult.size());
//
//    ASSERT_EQ(singleThreadResult[0].size(), openMPResult[0].size());
//
//    ASSERT_EQ(singleThreadResult, openMPResult);
//}
//
//
//TEST_F(computerTest, candidateSetNormalModeTest){
//
//    vector2D candidateSetSingleThread;
//    vector2D candidateSetOpenMP;
//
//    candidateSetSingleThread = mlsh.getCandidateSet();
//
//    mlsh.clearCollisionMatrix();
//    mlsh.clearCandidateSet();
//    mlsh.clearHashMatrix();
//
//    mlsh.setUseMultiThread(true);
//
//    candidateSetOpenMP = mlsh.getCandidateSet();
//
//    //check size -- should be empty
//    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
//    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());
//
//    //check values
//    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);
//
//}
//
//
//TEST_F(computerTest, candidateSetQuickModeTest){
//
//    vector2D candidateSetSingleThread;
//    vector2D candidateSetOpenMP;
//
//    mlsh.setComputeMode(1);
//
//    candidateSetSingleThread = mlsh.getCandidateSet();
//
//    //clear everything stored, make sure it will recalculate
//    mlsh.clearCollisionMatrix();
//    mlsh.clearCandidateSet();
//    mlsh.clearHashMatrix();
//
//    mlsh.setUseMultiThread(true);
//
//    candidateSetOpenMP = mlsh.getCandidateSet();
//
//    //check size -- should be empty
//    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
//    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());
//
//    //check values
//    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);
//
//}
//
//
//
//
//


