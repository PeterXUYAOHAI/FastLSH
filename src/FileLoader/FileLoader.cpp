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
    FileLoader.cpp
    Purpose: This is the source file of FileLoader class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <fstream>
#include <iostream>
#include "../../include/otherClasses/FileLoader.h"


vector2D FileLoader::ssToVector(size_t row, size_t col) {

    vector2D data;
    std::string value;
    std::string num;

    for (int i = 0; i < row; i++) {
        vector1D temp(col, 0);
        data.push_back(temp);
    }

    //parse csv file style
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


vector2D FileLoader::loadFile(const char* filePath, size_t row, size_t col){
    //load to stringstream then convert to vector
    loadToSS(filePath);
    return ssToVector(row, col);
}