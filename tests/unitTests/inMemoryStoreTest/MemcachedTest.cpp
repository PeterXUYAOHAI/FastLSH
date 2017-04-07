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
    MemcachedTest.cpp
    Purpose: This file is the Google test tests the Memcached functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/


#include <gtest/gtest.h>
#include "../../../include/LSH.h"
#include <chrono>

//marco to shorten the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class MemcachedTest:public ::testing::Test{

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
 * check if system give right response when the memcached program is not exist
 */
TEST_F(MemcachedTest, noExistTest){

    ASSERT_EQ(mlsh.saveHashNToMemcached("localhost", 11211, 0), 1);
}

/**
 * check if the data from memcached is in consistant with the origin data
 */
TEST_F(MemcachedTest, resultTest){

    mlsh.getCandidateSet();

    vector2D hashN = mlsh.hashMatrixN;


    t1 = now();
    ASSERT_EQ(mlsh.saveHashNToMemcached("localhost", 11211, 0), 0);
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for saving to memcache\n";

    t1 = now();
    mlsh.readHashNFromMemcached("localhost", 11211, mlsh.runID);
    t2 = now();

    duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for reading from memcache\n";

    //compare if re-readed hashMatrix are same with the original one
    ASSERT_EQ(hashN.size(), mlsh.hashMatrixN.size());
    ASSERT_EQ(hashN[0].size(), mlsh.hashMatrixN[0].size());

    //check each value
    for (int i = 0; i < mlsh.ph.N; ++i) {
        for (int j = 0; j < mlsh.ph.L; ++j) {
            ASSERT_NEAR(hashN[i][j], mlsh.hashMatrixN[i][j],0.5);
        }
    }

}



