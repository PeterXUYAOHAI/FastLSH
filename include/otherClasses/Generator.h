//
// Created by peter on 17-3-9.
//

#ifndef FASTLSH_GENERATOR_H
#define FASTLSH_GENERATOR_H

#include "Computer.h"

class Generator{

public:
//    Generator();
    Generator(ParameterHolder ph);
//    ~Generator();
    void changeComputer(int multiThreadMode, int computeMode);
    vector2D generateHash(vector2D dataset, size_t pNum);
    vector2D generateCollision(vector2D hMatrixN, vector2D hMatrixQ);
    vector2D generateCandidate(vector2D collisionMatrix);
    vector2D generateCandidate(vector2D hMatrixN, vector2D hMatrixQ);


private:

    Computer *theComputer;
    ParameterHolder ph;
};




#endif //FASTLSH_GENERATOR_H
