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
    HdfsLoader.cpp
    Purpose: This is the source file of HDFSFileLoader class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/
#include <fstream>
#include <hdfs.h>
#include <iostream>
#include "../../include/otherClasses/FileLoader.h"

void HdfsLoader::loadToSS(const char* filePath) {

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

void HdfsLoader::printLoadMode() {
    std::cout<<"HDFS (Hadoop File System)";
}
