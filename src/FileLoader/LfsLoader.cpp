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
    LfsLoader.cpp
    Purpose: This is the source file of LfsFileLoader class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/
#include <fstream>
#include <iostream>
#include "../../include/otherClasses/FileLoader.h"

void LfsLoader::loadToSS(const char* filePath) {
    //read ifstream into stringstream
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
