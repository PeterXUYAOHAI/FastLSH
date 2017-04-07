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
    HdfsTest.cpp
    Purpose: This file is the Google test tests the HdfsTest functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include <chrono>

#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class HdfsTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 1000, 57, 200, 1, 1.2,  100);
    }

    LSH mlsh;

    //varibale to hold read data
    vector2D setNLinux;
    vector2D setQLinux;
    vector2D setNHdfs;
    vector2D setQHdfs;

    //prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
};

/**
 * see if it can read from HDFS without exception,
 */
TEST_F(HdfsTest, readTest) {

    t1 = now();
    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv",0);
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs to read N set Linux File System\n";

    setNLinux = mlsh.setN;

    t1 = now();
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv",0);
    t2 = now();

    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs to read Q set Linux File System\n";
    setQLinux = mlsh.setQ;

    mlsh.setUseHdfs(true);

    t1 = now();
    mlsh.loadSetN("/dataset1000NoIndex.csv",0);
    t2 = now();

    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs to read N set HDFS\n";

    setNHdfs = mlsh.setN;

    t1 = now();
    mlsh.loadSetQ("/dataset1000NoIndex.csv",0);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs to read Q set HDFS\n";

    setQHdfs = mlsh.setQ;
}

/**
 * check if the readed file is of right size
 */
TEST_F(HdfsTest, sizeTest){

    ASSERT_EQ(setNLinux.size(), setNHdfs.size());
    ASSERT_EQ(setQLinux.size(), setQHdfs.size());
}


/**
 * check if the readed file's values are right
 */
TEST_F(HdfsTest, resultTest) {

    ASSERT_EQ(setNLinux, setNHdfs);
    ASSERT_EQ(setQLinux, setQHdfs);

}








