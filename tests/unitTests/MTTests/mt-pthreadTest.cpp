//
// Created by peter on 17-2-10.
//


#include <gtest/gtest.h>
#include "../../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class pthreadTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
//        mlsh = LSH(300000, 57, 200, 1, 1.2, 1000);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
//        mlsh.loadSetN("/home/peter/FYP/dataset300000NoIndex.csv", 0);
//        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;

    // prepare timerY
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;


};

TEST_F(pthreadTest, hashValueTest){
    vector2D singleThreadResult;
    vector2D stdthreadResult;

    t1 = now();
    vector2D hashQ = mlsh.computeHash(mlsh.setQ, mlsh.ph.Q);
    t2 = now();
    auto duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashQ_singeThread\n";

    t1 = now();
    vector2D hashN = mlsh.computeHash(mlsh.setN, mlsh.ph.N);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashN_singeThread\n";

    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(2);

    t1 = now();
    vector2D hashQ_mt = mlsh.computeHash_pthread(mlsh.setQ, mlsh.ph.Q);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashQ_pthread\n";


    t1 = now();
    vector2D hashN_mt = mlsh.computeHash_pthread(mlsh.setN, mlsh.ph.N);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashN_pthread\n";

    mlsh.setDefault();
    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQ_mt);
    ASSERT_EQ(hashN, hashN_mt);
}

TEST_F(pthreadTest, resultTest){
    vector2D singleThreadResult;
    vector2D pthreadResult;

    t1 = now();
    singleThreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for singleThread\n";

    mlsh.clearCollisionMatrix();
    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(2);

    t1 = now();
    pthreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for pThread\n";

    mlsh.setDefault();

    //compare if the two collision counting are same
    ASSERT_EQ(singleThreadResult.size(), pthreadResult.size());

    ASSERT_EQ(singleThreadResult[0].size(), pthreadResult[0].size());

    ASSERT_EQ(singleThreadResult, pthreadResult);

}

TEST_F(pthreadTest, candidateSetNormalModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetpthread;

    candidateSetSingleThread = mlsh.getCandidateSet();

    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(2);

    candidateSetpthread = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetpthread.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetpthread[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetpthread);

}


TEST_F(pthreadTest, candidateSetQuickModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetpthread;

    mlsh.setComputeMode(1);

    candidateSetSingleThread = mlsh.getCandidateSet();

    //clear everything stored, make sure it will recalculate
    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(2);

    candidateSetpthread = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetpthread.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetpthread[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetpthread);

}


