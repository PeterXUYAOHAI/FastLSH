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
    StdthreadTest.cpp
    Purpose: This file is the Google test tests the stdthread functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/


#include <gtest/gtest.h>
#include "../../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class stdthreadTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000,1000, 57, 200, 1, 1.2,  100);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;

    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;


};

/**
 * check if the stdthread result is the same as singleThread mode
 */
TEST_F(stdthreadTest, resultTest){
    vector2D singleThreadResult;
    vector2D stdthreadResult;

    t1 = now();
    singleThreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for singleThread\n";

    mlsh.setThreadMode(1);

    t1 = now();
    stdthreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for stdThread\n";

    mlsh.setDefault();

    //compare if the two collision counting are same
    ASSERT_EQ(singleThreadResult.size(), stdthreadResult.size());

    ASSERT_EQ(singleThreadResult[0].size(), stdthreadResult[0].size());

    ASSERT_EQ(singleThreadResult, stdthreadResult);

}

/**
 * check the stdthread version of Normal Mode result is same as singleThread
 */
TEST_F(stdthreadTest, candidateSetNormalModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetStdThread;

    candidateSetSingleThread = mlsh.getCandidateSet();

    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setThreadMode(1);

    candidateSetStdThread = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetStdThread.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetStdThread[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetStdThread);

}

/**
 * check the stdthread version of Quick Mode result is same as singleThread
 */
TEST_F(stdthreadTest, candidateSetQuickModeTest){

    vector2D candidateSetSingleThread;
    vector2D candidateSetStdThread;

    mlsh.setComputeMode(1);

    candidateSetSingleThread = mlsh.getCandidateSet();

    //clear everything stored, make sure it will recalculate
    mlsh.clearCollisionMatrix();
    mlsh.clearCandidateSet();
    mlsh.clearHashMatrix();

    mlsh.setThreadMode(1);

    candidateSetStdThread = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSetSingleThread.size(),candidateSetStdThread.size());
    ASSERT_EQ(candidateSetSingleThread[0].size(),candidateSetStdThread[0].size());

    //check values
    ASSERT_EQ(candidateSetSingleThread,candidateSetStdThread);

}



