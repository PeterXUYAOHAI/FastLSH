//
// Created by peter on 17-2-15.
//
#include <gtest/gtest.h>
#include "../../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class RedisTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;

    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;


};

TEST_F(RedisTest, noExistTest){

    ASSERT_EQ(mlsh.saveHashNToRedis("127.0.0.1", 6379), 1);
}


TEST_F(RedisTest, resultTest){

    mlsh.getCandidateSet();
    vector2D hashN = mlsh.hashMatrixN;


    t1 = now();
    ASSERT_EQ(mlsh.saveHashNToRedis("127.0.0.1", 6379), 0);
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for saving to memcache\n";

    t1 = now();
    mlsh.readHashNFromRedis("127.0.0.1", 6379, mlsh.runID);
    t2 = now();

    duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for reading from memcache\n";

    //compare if re-readed hashMatrix are same with the original one
    ASSERT_EQ(hashN.size(), mlsh.hashMatrixN.size());

    ASSERT_EQ(hashN[0].size(), mlsh.hashMatrixN[0].size());

    for (int i = 0; i < mlsh.ph.N; ++i) {
        for (int j = 0; j < mlsh.ph.L; ++j) {
            ASSERT_NEAR(hashN[i][j], mlsh.hashMatrixN[i][j],0.5);
        }
    }

}



