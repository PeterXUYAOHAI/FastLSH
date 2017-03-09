//
// Created by peter on 17-3-9.
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

        normalComputer = new ComputerSingleThreadNormal();
        quickComputer = new ComputerSingleThreadQuick();

//            mlsh.loadSetN("/home/peter/FYP/dataset300000NoIndex.csv", 0);
//            mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;
    Computer *normalComputer;
    Computer *quickComputer;
    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
};

TEST_F(computerTest, hashValueTest){
    vector2D singleThreadResult;

    vector2D hashQ = mlsh.computeHash(mlsh.setQ, mlsh.Q);

    vector2D hashN = mlsh.computeHash(mlsh.setN, mlsh.N);

    vector2D hashQ_mt = mlsh.computeHash(mlsh.setQ, mlsh.Q);

    vector2D hashN_mt = mlsh.computeHash_openmp(mlsh.setN, mlsh.N);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQ_mt);
    ASSERT_EQ(hashN, hashN_mt);
}



TEST_F(computerTest, resultTest) {
    vector2D singleThreadResult;

    openMPResult = mlsh.getCollisionMatrix();



    mlsh.clearCollisionMatrix();

    mlsh.setUseMultiThread(false);

    singleThreadResult = mlsh.getCollisionMatrix();


    //compare if the two collision counting are same
    ASSERT_EQ(singleThreadResult.size(), openMPResult.size());

    ASSERT_EQ(singleThreadResult[0].size(), openMPResult[0].size());

    ASSERT_EQ(singleThreadResult, openMPResult);
}


TEST_F(computerTest, candidateSetNormalModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetOpenMP;

    candidateSetSingleThread = mlsh.getCandidateSet();

    candidateSetOpenMP = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);

}


TEST_F(computerTest, candidateSetQuickModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetOpenMP;

    candidateSetSingleThread = mlsh.getCandidateSet();

    candidateSetOpenMP = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);

}

