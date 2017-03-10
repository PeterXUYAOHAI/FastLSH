//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class OpenMPTest:public ::testing::Test{

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

    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
};


TEST_F(OpenMPTest, resultTest) {
    vector2D singleThreadResult;
    vector2D openMPResult;

    mlsh.setThreadMode(1);


    t1 = now();
    openMPResult = mlsh.getCollisionMatrix();
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for openMP\n";


    mlsh.clearCollisionMatrix();

    mlsh.setThreadMode(1);

    t1 = now();
    singleThreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for singleThread\n";

    //compare if the two collision counting are same
    ASSERT_EQ(singleThreadResult.size(), openMPResult.size());

    ASSERT_EQ(singleThreadResult[0].size(), openMPResult[0].size());

    ASSERT_EQ(singleThreadResult, openMPResult);
}


TEST_F(OpenMPTest, candidateSetNormalModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetOpenMP;

    candidateSetSingleThread = mlsh.getCandidateSet();

    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setThreadMode(1);

    candidateSetOpenMP = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);

}


TEST_F(OpenMPTest, candidateSetQuickModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetOpenMP;

    mlsh.setComputeMode(1);

    candidateSetSingleThread = mlsh.getCandidateSet();

    //clear everything stored, make sure it will recalculate
    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setThreadMode(1);

    candidateSetOpenMP = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetOpenMP.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetOpenMP[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetOpenMP);

}







