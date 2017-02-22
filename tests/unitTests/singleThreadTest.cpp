//
// Created by peter on 17-2-22.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"


class singleThreadTest: public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);

        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }

    LSH mlsh;
};


TEST_F(singleThreadTest, collisionMatrixGenTest){


    ASSERT_EQ(mlsh.collisionMatrix.size(),0);

    mlsh.getCollisionMatrix();

    //check size -- here we only checked size, more checking is needed in the future
    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.N);

}


TEST_F(singleThreadTest, candidateSetTest){

    vector2D candidateSet;

    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}


TEST_F(singleThreadTest, candidateSetQuickModeTest){

    vector2D candidateSet;

    mlsh.setComputeMode(1);
    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    for (int i = 0; i < 1000; ++i) {
    ASSERT_GE(candidateSet[i].size(), 0);
    }

}


TEST_F(singleThreadTest, genCandidateMultiModeTest){

    vector2D candidateSetNormal;
    vector2D candidateSetQuick;

    candidateSetNormal = mlsh.getCandidateSet();

    mlsh.clearCandidateSet();

    mlsh.setComputeMode(1);

    candidateSetQuick = mlsh.getCandidateSet();
    //check size -- should be empty
    ASSERT_EQ(candidateSetNormal,candidateSetQuick);
}
