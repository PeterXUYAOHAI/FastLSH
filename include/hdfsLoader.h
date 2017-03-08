//
// Created by peter on 17-3-8.
//

#ifndef FASTLSH_HDFSLOADER_H
#define FASTLSH_HDFSLOADER_H

#include "fileLoader.h"

class hdfsLoader: public fileLoader {

public:
    hdfsLoader(char* filePath, size_t row, size_t col, size_t fileSize):
            fileLoader(filePath,row,col), fileSize(fileSize){}

private:
    size_t fileSize;
    void loadToSS();

};







#endif //FASTLSH_HDFSLOADER_H
