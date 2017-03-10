//
// Created by peter on 17-2-22.
//

#include <gtest/gtest.h>
#include <chrono>
#include "../../include/LSH.h"


//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>



class SingleThreadTest: public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);

        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }

    LSH mlsh;
    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;

};


TEST_F(SingleThreadTest, collisionMatrixGenTest){


    ASSERT_EQ(mlsh.collisionMatrix.size(),0);

    mlsh.getCollisionMatrix();

    //check size -- here we only checked size, more checking is needed in the future
    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.ph.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.ph.N);

}


TEST_F(SingleThreadTest, candidateSetNormalModeTest){

    vector2D candidateSet;

    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}


TEST_F(SingleThreadTest, candidateSetQuickModeTest){

    vector2D candidateSet;

    mlsh.setComputeMode(1);
    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}


TEST_F(SingleThreadTest, genCandidateMultiModeTest){

    vector2D candidateSetNormal;
    vector2D candidateSetQuick;

    t1 = now();
    candidateSetNormal = mlsh.getCandidateSet();
    t2 = now();
    auto duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs getCandidateNormal_singeThread\n";

    //clear everything stored, make sure it will recalculate
    mlsh.clearCandidateSet();
    mlsh.clearCollisionMatrix();
    mlsh.clearHashMatrix();

    mlsh.setComputeMode(1);

    t1 = now();
    candidateSetQuick = mlsh.getCandidateSet();
    t2 = now();

    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs getCandidateQuick_singeThread\n";


    //check size -- should be empty
    ASSERT_EQ(candidateSetNormal,candidateSetQuick);
}
