//
// Created by peter on 17-3-8.
//

#ifndef FASTLSH_LFSLOADER_H
#define FASTLSH_LFSLOADER_H

#endif //FASTLSH_LFSLOADER_H

#include <vector>
#include "fileLoader.h"

class lfsLoader: public fileLoader {

public:
    lfsLoader(char* filePath, size_t row, size_t col): fileLoader(filePath,row,col){}

private:
    void loadToSS();
};
