//
// Created by peter on 17-3-8.
//

#ifndef FASTLSH_HDFSLOADER_H
#define FASTLSH_HDFSLOADER_H

#include "fileLoader.h"

class hdfsLoader: public fileLoader {

private:
    size_t fileSize;
    virtual void loadToSS(char* filePath);

};







#endif //FASTLSH_HDFSLOADER_H
