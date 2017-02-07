//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"


class metaTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000);
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