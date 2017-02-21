//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"


class metaTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
    }

    LSH mlsh;
};


TEST_F(metaTest, randGeneTest){

    mlsh.generateRandomLine();

    ASSERT_EQ(mlsh.randomLine.size(), mlsh.L);

    ASSERT_EQ(mlsh.randomLine[0].size(), mlsh.K);

    ASSERT_EQ(mlsh.randomLine[0][0].size(), mlsh.D);


    vector1D randVec = mlsh.generateUniformRandomVector(7, 10);

    ASSERT_EQ(randVec.size(), 7);

    //every num should smaller than maxium(generateUNiformRandomVector second variable)
    for(std::vector<double>::iterator it = randVec.begin(); it != randVec.end(); ++it) {
        ASSERT_LE(*it, 10);
    }


}


TEST_F(metaTest, linuxReadTest){

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


TEST_F(metaTest, collisionMatrixGenTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    ASSERT_EQ(mlsh.collisionMatrix.size(),0);

    mlsh.generateCollisionMatrix();

    //check size -- here we only checked size, more checking is needed in the future
    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.N);

}


TEST_F(metaTest, clearcollisionMatrixGenTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    mlsh.generateCollisionMatrix();

    ASSERT_EQ(mlsh.collisionMatrix.size(),mlsh.Q);
    ASSERT_EQ(mlsh.collisionMatrix[0].size(),mlsh.N);

    mlsh.clearCollisionMatrix();

    //check size -- should be empty
    ASSERT_EQ(mlsh.collisionMatrix.size(),0);
}


TEST_F(metaTest, candidateSetTest){

    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    vector2D candidateSet;

    candidateSet = mlsh.getCandidateSet();

    //check size -- should be empty
    ASSERT_EQ(candidateSet.size(),1000);

    for (int i = 0; i < 1000; ++i) {
        ASSERT_GE(candidateSet[i].size(), 0);
    }

}






//just legency
//TEST_F(metaTest, hashValueTest){
//
//    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);
//
//    vector2D setQN;
//    vector2D hashQ;
//
//    setQN = mlsh.normalize(mlsh.setQ);
//    hashQ = mlsh.computeHash(setQN,mlsh.Q);
//
//    double temp;
//
//
//    for (int i = 0; i < mlsh.Q; ++i) {
//        for (int j = 0; j < mlsh.L; ++j) {
//            std::cout<<hashQ[i][j]<<"\n";
//            ASSERT_EQ(modf(hashQ[i][j], &temp),0);
//        }
//    }
//}