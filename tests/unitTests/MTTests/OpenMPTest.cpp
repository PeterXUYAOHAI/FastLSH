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
    OpenMPTest.cpp
    Purpose: This file is the Google test tests the OpenMP functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/


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
 * check if the OpenMP result is the same as singleThread mode
 */
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

/**
 * check the OpenMP version of Normal Mode result is same as singleThread
 */
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


/**
 * check the OpenMP version of Quick Mode result is same as singleThread
 */
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







