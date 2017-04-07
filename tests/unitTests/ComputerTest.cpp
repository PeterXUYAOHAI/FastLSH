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
    ComputerTest.cpp
    Purpose: This file is the Google test tests the Computer.h functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <gtest/gtest.h>
#include "../../include/LSH.h"

class ComputerTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 1000, 57, 200, 1, 1.2,  100);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
    }
    LSH mlsh;
    Computer *theNormalComputer;
    Computer *theQuickComputer;
};

/**
 * check if the singleThread compute through computer class is right
 */
TEST_F(ComputerTest, singleThreadTest){

    theNormalComputer = new ComputerSingleThreadNormal(mlsh.ph);
    theQuickComputer = new ComputerSingleThreadQuick(mlsh.ph);

    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theNormalComputer->computeCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    vector2D hashQComputer = theNormalComputer->computeHash(mlsh.setQ,mlsh.ph.Q);
    vector2D hashNComputer = theNormalComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashNQuickComputer = theQuickComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashQ = mlsh.hashMatrixQ;
    vector2D hashN = mlsh.hashMatrixN;


    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theNormalComputer->computeCandidate(resultOrigin);
    vector2D candidateQuick = theQuickComputer->computeCandidate(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    delete theNormalComputer;
    delete theQuickComputer;

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQComputer);
    ASSERT_EQ(hashN, hashNComputer);
    ASSERT_EQ(hashN, hashNQuickComputer);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
    ASSERT_EQ(candidateOrigin, candidateQuick);
}


/**
 * check if the openMP compute through computer class is right
 */
TEST_F(ComputerTest, openMPTest){

    theNormalComputer = new ComputerOpenMPNormal(mlsh.ph);
    theQuickComputer = new ComputerOpenMPQuick(mlsh.ph);

    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theNormalComputer->computeCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);


    vector2D hashQComputer = theNormalComputer->computeHash(mlsh.setQ,mlsh.ph.Q);
    vector2D hashNComputer = theNormalComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashNQuickComputer = theQuickComputer->computeHash(mlsh.setN,mlsh.ph.N);


    vector2D hashQ = mlsh.hashMatrixQ;
    vector2D hashN = mlsh.hashMatrixN;


    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theNormalComputer->computeCandidate(resultOrigin);
    vector2D candidateQuick = theQuickComputer->computeCandidate(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    delete theNormalComputer;
    delete theQuickComputer;

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQComputer);
    ASSERT_EQ(hashN, hashNComputer);
    ASSERT_EQ(hashN, hashNQuickComputer);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
    ASSERT_EQ(candidateOrigin, candidateQuick);
}


/**
 * check if the stdThread compute through computer class is right
 */
TEST_F(ComputerTest, StdThreadTest){

    theNormalComputer = new ComputerStdThreadNormal(mlsh.ph);
    theQuickComputer = new ComputerStdThreadQuick(mlsh.ph);

    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theNormalComputer->computeCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    vector2D hashQComputer = theNormalComputer->computeHash(mlsh.setQ,mlsh.ph.Q);
    vector2D hashNComputer = theNormalComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashNQuickComputer = theQuickComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashQ = mlsh.hashMatrixQ;
    vector2D hashN = mlsh.hashMatrixN;

    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theNormalComputer->computeCandidate(resultOrigin);
    vector2D candidateQuick = theQuickComputer->computeCandidate(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    delete theNormalComputer;
    delete theQuickComputer;

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQComputer);
    ASSERT_EQ(hashN, hashNComputer);
    ASSERT_EQ(hashN, hashNQuickComputer);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
    ASSERT_EQ(candidateOrigin, candidateQuick);
}


/**
 * check if the Pthread compute through computer class is right
 */
TEST_F(ComputerTest, PthreadTest){

    theNormalComputer = new ComputerPthreadNormal(mlsh.ph);
    theQuickComputer = new ComputerPthreadQuick(mlsh.ph);

    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theNormalComputer->computeCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    vector2D hashQComputer = theNormalComputer->computeHash(mlsh.setQ,mlsh.ph.Q);
    vector2D hashNComputer = theNormalComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashNQuickComputer = theQuickComputer->computeHash(mlsh.setN,mlsh.ph.N);

    vector2D hashQ = mlsh.hashMatrixQ;
    vector2D hashN = mlsh.hashMatrixN;

    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theNormalComputer->computeCandidate(resultOrigin);
    vector2D candidateQuick = theQuickComputer->computeCandidate(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    delete theNormalComputer;
    delete theQuickComputer;

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQComputer);
    ASSERT_EQ(hashN, hashNComputer);
    ASSERT_EQ(hashN, hashNQuickComputer);
    ASSERT_EQ(resultOrigin, resultNormal);

    ASSERT_EQ(candidateOrigin, candidateQuick);
}

