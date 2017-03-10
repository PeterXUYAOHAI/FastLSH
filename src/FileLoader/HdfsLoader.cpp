//
// Created by peter on 17-3-10.
//

#include <fstream>
#include <hdfs.h>
#include <iostream>
#include "../../include/otherClasses/FileLoader.h"

void HdfsLoader::loadToSS(char* filePath) {

    int exists; //file exist flag
    char* buffer; //buffer store file read from hdfs
    size_t bufferSize; //bufferSize, if not specify define, set as 500000
    hdfsFS fs; //hdfsFS object
    hdfsFile readFile; //file read from hdfs

    //if fileSize not specify, set as default-500000
    if(fileSize!=0)
        bufferSize = fileSize;
    else
        bufferSize = 500000;

    //hdfs read procedure
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

    //pip file from hdfs to stringstream
    ss<< buffer;

    //close hdfs related object, free memory
    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);

}