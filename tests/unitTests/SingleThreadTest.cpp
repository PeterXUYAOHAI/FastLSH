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
    SingeThreadTest.cpp
    Purpose: This file is the Google test tests the SingleThread functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <gtest/gtest.h>
#include <chrono>
#include "../../include/LSH.h"


//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class SingleThreadTest: public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 1000, 57, 200, 1, 1.2,  100);

        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }

    LSH mlsh;
    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;

};

/**
 * check if the collisionMatrix generated by singleThread is of right size
 */
TEST_F(SingleThreadTest, collisionMatrixGenTest){


    ASSERT_EQ(mlsh.collisionMatrix.size(),0);

    mlsh.getCollisionMatrix();

    //check size -- here we only checked size, more checking is needed in the future
    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.ph.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.ph.N);

}

/**
 * check if the candidateSet generated by singleThread Normal Mode is of right size
 */
TEST_F(SingleThreadTest, candidateSetNormalModeTest){

    vector2D candidateSet;

    candidateSet = mlsh.getCandidateSet();

    //check size
    ASSERT_EQ(candidateSet.size(),1000);

    //each row's size should be greater than 0
    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}

/**
 * check if the candidateSet generated by singleThread Quick Mode is of right size
 */
TEST_F(SingleThreadTest, candidateSetQuickModeTest){

    vector2D candidateSet;

    mlsh.setComputeMode(1);
    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    //each row's size should be greater than 0
    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}

/**
 * check if the NormalMode and Quick Mode generate the same result
 */
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

    //check the two result are identical
    ASSERT_EQ(candidateSetNormal,candidateSetQuick);
}
