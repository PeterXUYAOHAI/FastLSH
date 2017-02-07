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

//    LSH mlsh(1000, 57, 200, 1, 1.2, 1000);

    {
        // test use multithread - openMP
//        mlsh.setUseMultiThread(true);
//        mlsh.loadSetN("/dataset1000NoIndex.csv", 0);
//        mlsh.loadSetQ("/dataset1000NoIndex.csv", 0);
//
//        vector2D collisionMatrix;
//        collisionMatrix = mlsh.getCollisionMatrix();
//        std::cout << collisionMatrix.size();
//        std::cout << collisionMatrix[0].size();
//        std::cout << collisionMatrix[0][200];

    }


    {
        // test use hdfs
//        mlsh.setUseHdfs(true);
//        mlsh.loadSetN("/dataset1000NoIndex.csv", 0);
//        mlsh.loadSetQ("/dataset1000NoIndex.csv", 0);
//
//        vector2D collisionMatrix;
//        collisionMatrix = mlsh.getCollisionMatrix();
//
//        std::cout << collisionMatrix.size();
//        std::cout << collisionMatrix[0].size();
//        std::cout << collisionMatrix[0][200];
    }

//    {
        //hdfs sample flow
//        hdfsFS fs = hdfsConnect("default", 0);
//
//        if (!fs) {
//            fprintf(stderr, "Failed to connect to hdfs.\n");
//            exit(-1);
//        }
//
//        int ne = 5;
//        hdfsFileInfo *fInfo = hdfsGetPathInfo(fs, "/");
//        hdfsFileInfo *dirInfo = hdfsListDirectory(fs, "/", &ne);
//        std::cout << fInfo->mName;
//        std::cout << dirInfo[0].mName << std::endl;
//
//
//        hdfsFile readFile;
//        char *buffer;
//        tSize bufferSize;
//
//        readFile = hdfsOpenFile(fs, "/dataset1000NoIndex.csv", O_RDONLY, 0, 0, 0);
//
//
//        if (!hdfsFileIsOpenForRead(readFile)) {
//            fprintf(stderr, "hdfsFileIsOpenForRead: we just opened a file "
//                    "with O_RDONLY, and it did not show up as 'open for "
//                    "read'\n");
//            exit(-1);
//        }
//
//        buffer = (char *) malloc(sizeof(char) * 300000);
//        bufferSize = 50000;
//        tSize num_read_bytes = hdfsRead(fs, readFile, (void *) buffer, bufferSize);
//
//        std::cout << num_read_bytes << std::endl;
//
//        for (int i = 0; i < 200; ++i) {
//            std::cout << buffer[i];
//        }
//
//
//        std::stringstream ss;
//        ss << buffer;
//        std::string value;
//        getline(ss, value, ',');
//        getline(ss, value, ',');
//        std::cout << std::endl << value;
//        free(buffer);
//        hdfsCloseFile(fs, readFile);
//        hdfsDisconnect(fs);
//
//    }

    {
        //sample read from linux file system

//    // Para set:(size_t N, size_t D, size_t L, size_t K, double W, size_t Q)
//    LSH mlsh(1000, 57, 200, 1, 1.2, 1000);
//
//    //here we use same set for N and query set
//    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv");
//    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv");
//
//    vector2D collisionMatrix;
//    collisionMatrix = mlsh.getCollisionMatrix();

//    std::cout<<collisionMatrix.size();
//    std::cout<<collisionMatrix[0].size();

    }

//  gengerate random test

//    LSH mlsh;

//    vector3D temp = mlsh.generateRandomLine(200,3,50);
//    std::cout<<temp.size();
//    std::cout<<temp[0].size();
//    std::cout<<temp[0][0].size();
//    std::cout<<temp[0][0][0];
//    std::vector<double> v = mlsh.generateUniformRandomVector(10,1.3);
//    std::cout<<v.size();
//    std::cout<<v[0];

// load file test
//    {
//    LSH mlsh;
//    std::vector<std::vector<double>>  test;
//    test = mlsh.loadData("../tests/dataset/dataset1000NoIndex.csv", 1000, 57);
//    std::cout<<test[0][2];
//    }

//compute hashmatrix
//    vector2D hashmatrix;
//    hashmatrix = mlsh.computeHash(test,200,1.2,1000,57,3);
//    std::cout<<hashmatrix[0][0]<<std::endl;
//    std::cout<<hashmatrix.size()<<std::endl;
//    std::cout<<hashmatrix[0].size()<<std::endl;



}