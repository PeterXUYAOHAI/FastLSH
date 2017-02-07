#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include "../include/LSH.h"
#include "../include/hdfs.h"
#include "hdfs.h"
#include <gtest/gtest.h>

int main (int argc, char **argv){
    std::cout<<"hello world!";

    //run the registered tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();


//compute hashmatrix
//    vector2D hashmatrix;
//    hashmatrix = mlsh.computeHash(test,200,1.2,1000,57,3);
//    std::cout<<hashmatrix[0][0]<<std::endl;
//    std::cout<<hashmatrix.size()<<std::endl;
//    std::cout<<hashmatrix[0].size()<<std::endl;

}