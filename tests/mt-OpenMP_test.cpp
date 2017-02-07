//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../include/LSH.h"


class openMPTest:public ::testing::Test{

    protected:
        virtual void SetUp() {
            mlsh = LSH(1000, 57, 200, 1, 1.2, 1000);
            mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
            mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
        }
    LSH mlsh;
};

TEST_F(openMPTest, hashValueTest){
    vector2D singleThreadResult;
    vector2D openMPResult;

    vector2D hashQ = mlsh.computeHash(mlsh.setQ, mlsh.Q);
    vector2D hashN = mlsh.computeHash(mlsh.setN, mlsh.N);

    mlsh.setUseMultiThread(true);

    vector2D hashQ_mt = mlsh.computeHash(mlsh.setQ, mlsh.Q);
    vector2D hashN_mt = mlsh.computeHash(mlsh.setN, mlsh.N);

    ASSERT_EQ(hashQ,hashQ_mt);
    ASSERT_EQ(hashN, hashN_mt);

}



TEST_F(openMPTest, resultTest) {
    vector2D singleThreadResult;
    vector2D openMPResult;

    mlsh.setUseMultiThread(true);
    openMPResult = mlsh.getCollisionMatrix();


    mlsh.setUseMultiThread(false);

    singleThreadResult = mlsh.getCollisionMatrix();



//
    ASSERT_EQ(singleThreadResult, openMPResult);
}








