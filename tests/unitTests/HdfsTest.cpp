//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include <chrono>

#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class HdfsTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
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

//see if can read without exception, also print the time used
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

TEST_F(HdfsTest, sizeTest){

    ASSERT_EQ(setNLinux.size(), setNHdfs.size());
    ASSERT_EQ(setQLinux.size(), setQHdfs.size());
}



TEST_F(HdfsTest, resultTest) {

    ASSERT_EQ(setNLinux, setNHdfs);
    ASSERT_EQ(setQLinux, setQHdfs);

}








