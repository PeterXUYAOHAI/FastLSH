//
// Created by peter on 17-2-7.
//

#include "../include/LSH.h"
#include "hdfs.h"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>


vector2D LSH::loadDataFromHDFS(char* filePath, int row, int col, int fileSize){

    int exists;
    char* buffer;
    int bufferSize;
    hdfsFS fs;
    vector2D data;
    hdfsFile readFile;
    std::stringstream ss;
    std::string value;
    std::string num;


    if(fileSize!=0)
        bufferSize = fileSize;
    else
        bufferSize = 300000;

    fs = hdfsConnectNewInstance("default", 0);
    if(!fs) {
        std::cout<< "Oops! Failed to connect to hdfs!\n";
        exit(-1);
    }

    exists = hdfsExists(fs, filePath);
    if(exists){
        std::cout<<"Failed to validate existence of "<<filePath<<std::endl;
        exit(-1);
    }

    readFile = hdfsOpenFile(fs, filePath, O_RDONLY, 0, 0, 0);
    if(!readFile){
        std::cout<<"Failed to open "<<filePath<<" for reading!\n";
        exit(-1);
    }

    if(!hdfsFileIsOpenForRead(readFile)){
        fprintf(stderr, "hdfsFileIsOpenForRead: we just opened a file "
                "with O_RDONLY, and it did not show up as 'open for "
                "read'\n");
        exit(-1);
    }

    fprintf(stderr, "hdfsAvailable: %d\n", hdfsAvailable(fs, readFile));

    buffer = (char*)malloc(sizeof(char) * bufferSize);

    hdfsRead(fs, readFile, (void*)buffer, bufferSize);

    ss<< buffer;

    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);


    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }

    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(ss, value, ',');
            else
                getline(ss,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    return data;

}