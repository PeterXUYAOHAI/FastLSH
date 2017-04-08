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
    FileLoader.h
    Purpose: This is the header file of FileLoader classes
    Patterns: strategy
    Computer is response for loading dataset in LSH, file will first been loaded into stringstream then parsed
    into 2D vector

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#ifndef FASTLSH_FILELOADER_H
#define FASTLSH_FILELOADER_H


#include <vector>
#include <cstdio>
#include <sstream>

typedef std::vector<std::vector<std::vector < double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;


/**
 * abstract class of FileLoader
 */
class FileLoader {
public:
    /**
     * load File
     * @param filePath
     * @param row number of rows of the matrix
     * @param col number of columns of the matrix
     * @return 2D vector<double> of the dataset
     */
    vector2D loadFile(const char* filePath, size_t row, size_t col);

    /**
     * print the loading mode
     */
    virtual void printLoadMode()=0;

protected:
    std::stringstream ss; //stringstream for processing loaded file
private:
    /**
     * load file into stringstream
     * @param filePath
     */
    virtual void loadToSS(const char* filePath)=0;

    /**
     * convert stringstream to 2D vector
     * @param row number of rows
     * @param col number of columns
     * @return 2D vector<double> of the dataset
     */
    vector2D ssToVector(size_t row, size_t col);

};


/**
 * FileLoader which loads file from Linux File System(LFS) derived from FileLoader
 */
class LfsLoader: public FileLoader {
    /**
     * print the loadMode - LFS
     */
    virtual void printLoadMode();
private:
    /**
     * load the file into stringstream
     * @param filePath
     */
    virtual void loadToSS(const char* filePath);

};

/**
 * FileLoader which loads file from Hadoop File System (HDFS) derived from FileLoader
 */
class HdfsLoader: public FileLoader {
    /**
     * print the loadMode - HDFS
     */
    virtual void printLoadMode();
private:
    size_t fileSize; // fileSize for HDFS loader reference

    /**
     * load file into stringstream
     * @param filePath
     */
    virtual void loadToSS(const char* filePath);

};


#endif //FASTLSH_FILELOADER_H
