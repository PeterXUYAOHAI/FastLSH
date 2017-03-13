//
// Created by peter on 17-3-10.
//

#include <fstream>
#include <iostream>
#include "../../include/otherClasses/FileLoader.h"

void LfsLoader::loadToSS(const char* filePath) {
    std::ifstream file;
    file.open(filePath);

    if(file) {
        ss << file.rdbuf();
        file.close();
    }
}

void LfsLoader::printLoadMode() {
    std::cout<<"LFS(Linux File System)";
}
