//
// Created by peter on 17-3-10.
//

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

vector2D FileLoader::loadFile(char* filePath, size_t row, size_t col){
    loadToSS(filePath);
    return ssToVector(row, col);
}