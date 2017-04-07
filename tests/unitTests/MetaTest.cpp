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
    MetaTest.cpp
    Purpose: This file is the Google test tests the basic functions of LSH class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <gtest/gtest.h>
#include "../../include/LSH.h"


class MetaTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 1000, 57, 200, 1, 1.2,  100);
    }

    LSH mlsh;
};


/**
 * check if the random generation function gives right result
 */
TEST_F(MetaTest, randGeneTest){

    mlsh.generateRandomLine();

    ASSERT_EQ(mlsh.ph.randomLine.size(), mlsh.ph.L);
    ASSERT_EQ(mlsh.ph.randomLine[0].size(), mlsh.ph.K);
    ASSERT_EQ(mlsh.ph.randomLine[0][0].size(), mlsh.ph.D);

    vector1D randVec = mlsh.generateUniformRandomVector(7, 10);

    ASSERT_EQ(randVec.size(), 7);

    //every num should be smaller than maxium(generateUNiformRandomVector second variable)
    for(std::vector<double>::iterator it = randVec.begin(); it != randVec.end(); ++it) {
        ASSERT_LE(*it, 10);
    }
}


/**
 * check if the random generation function gives right result
 */
TEST_F(MetaTest, linuxReadTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    //check size
    ASSERT_EQ(mlsh.setN.size(),1000);
    ASSERT_EQ(mlsh.setN.size(),1000);
    ASSERT_EQ(mlsh.setQ[0].size(),57);
    ASSERT_EQ(mlsh.setQ[0].size(),57);

    //check several value
    ASSERT_EQ(mlsh.setQ[0][0],1302);
    ASSERT_EQ(mlsh.setQ[0][0],1302);
}



/**
 * check if the clear function really release the memory
 */
TEST_F(MetaTest, clearCollisionMatrixTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    mlsh.getCollisionMatrix();

    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.ph.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.ph.N);

    mlsh.clearCollisionMatrix();
    //check size -- should be empty
    ASSERT_EQ(mlsh.collisionMatrix.size(),0);
}



/**
 * check if the clear function really release the memory
 */
TEST_F(MetaTest, clearTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    mlsh.getCandidateSet();

    ASSERT_EQ(mlsh.candidateSet.size(),mlsh.ph.Q);

    mlsh.clear();

    //check size -- should be empty
    ASSERT_EQ(mlsh.collisionMatrix.size(),0);
    ASSERT_EQ(mlsh.candidateSet.size(),0);
}



