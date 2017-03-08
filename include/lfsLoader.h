//
// Created by peter on 17-3-8.
//

#ifndef FASTLSH_LFSLOADER_H
#define FASTLSH_LFSLOADER_H

#endif //FASTLSH_LFSLOADER_H

#include <vector>
#include "fileLoader.h"

class lfsLoader: public fileLoader {

private:
    virtual void loadToSS(char* filePath);
};
