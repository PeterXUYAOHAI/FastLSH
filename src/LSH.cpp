//
// Created by peter on 17-2-4.
//
#include "../include/LSH.h"
#include <fstream>
#include <iostream>

typedef std::vector<std::vector<std::vector<double>>> vector3D;

std::vector<std::vector<double>> LSH::loadData(std::string filePath, size_t row, size_t col) {
    std::ifstream file;// declare file stream:
    file.open(filePath);
    std::vector<std::vector<double>> data;
    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }
    std::string value;
    std::string num;
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(file, value, ',');
            else
                getline(file,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    file.close();
    return data;
}



void computeHash(){
    vector3D 




}










