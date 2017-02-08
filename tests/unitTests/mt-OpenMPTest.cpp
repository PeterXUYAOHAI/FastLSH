//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include <chrono>

//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

class openMPTest:public ::testing::Test{

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

TEST_F(openMPTest, hashValueTest){
    vector2D singleThreadResult;
    vector2D openMPResult;

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

    t1 = now();
    vector2D hashQ_mt = mlsh.computeHash(mlsh.setQ, mlsh.Q);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashQ_openMP\n";


    t1 = now();
    vector2D hashN_mt = mlsh.computeHash(mlsh.setN, mlsh.N);
    t2 = now();
    duration = dcast( t2 - t1 ).count();
    std::cout <<duration << " μs computeHashN_openMP\n";

    mlsh.setDefault();

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQ_mt);
    ASSERT_EQ(hashN, hashN_mt);
}



TEST_F(openMPTest, resultTest) {
    vector2D singleThreadResult;
    vector2D openMPResult;

    mlsh.setUseMultiThread(true);


    t1 = now();
    openMPResult = mlsh.getCollisionMatrix();
    t2 = now();

    auto duration = dcast( t2 - t1 ).count();

    std::cout <<duration << " μs for openMP\n";


    mlsh.setUseMultiThread(false);

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








