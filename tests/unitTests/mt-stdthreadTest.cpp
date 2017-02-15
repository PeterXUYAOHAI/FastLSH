//
// Created by peter on 17-2-8.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class stdthreadTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;

    // prepare timer
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;


};

TEST_F(stdthreadTest, hashValueTest){
    vector2D singleThreadResult;
    vector2D stdthreadResult;


    t1 = now();
    vector2D hashQ = mlsh.computeHash(mlsh.setQ, mlsh.Q);
    t2 = now();
    auto duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashQ_singeThread\n";

    t1 = now();
    vector2D hashN = mlsh.computeHash(mlsh.setN, mlsh.N);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashN_singeThread\n";

    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(1);

    t1 = now();
    vector2D hashQ_mt = mlsh.computeHash_stdthread(mlsh.setQ, mlsh.Q);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashQ_stdthread\n";


    t1 = now();
    vector2D hashN_mt = mlsh.computeHash_stdthread(mlsh.setN, mlsh.N);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashN_stdthread\n";

    mlsh.setDefault();
    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQ_mt);
    ASSERT_EQ(hashN, hashN_mt);
}

TEST_F(stdthreadTest, resultTest){
    vector2D singleThreadResult;
    vector2D stdthreadResult;

    t1 = now();
    singleThreadResult = mlsh.getCollisionMatrix();
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for singleThread\n";

    mlsh.setUseMultiThread(true);
    mlsh.setMultiThreadMode(1);

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



