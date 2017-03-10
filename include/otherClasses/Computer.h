//
// Created by peter on 17-3-9.
//

#ifndef FASTLSH_COMPUTER_H
#define FASTLSH_COMPUTER_H

#include <vector>
#include <cstdio>
#include <sstream>
#include "ParameterHolder.h"

typedef std::vector<std::vector<std::vector < double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

class Computer{

public:
    Computer(){};
    Computer(ParameterHolder ph) : ph(ph) {};
    virtual vector2D computeHash(vector2D dataset, size_t pNum)=0;
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ){};
    virtual vector2D computeCandidate(vector2D collisionMatrix){};
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ){};

protected:
    ParameterHolder ph;
};


class ComputerSingleThread: public Computer{

public:
    ComputerSingleThread(ParameterHolder ph):
            Computer(ph){};

    virtual vector2D computeHash(vector2D dataset, size_t pNum);

};


class ComputerSingleThreadNormal: public  ComputerSingleThread{

public:
    ComputerSingleThreadNormal(ParameterHolder ph):
            ComputerSingleThread(ph){};

    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);
    virtual vector2D computeCandidate(vector2D collisionMatrix);
};


class ComputerSingleThreadQuick: public  ComputerSingleThread{

public:
    ComputerSingleThreadQuick(ParameterHolder ph):
            ComputerSingleThread(ph){};

    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);
};



class ComputerOpenMP: public Computer{

public:
    ComputerOpenMP(ParameterHolder ph):Computer(ph){};
    virtual vector2D computeHash(vector2D dataset, size_t pNum);
};

class ComputerOpenMPNormal: public ComputerOpenMP{

public:
    ComputerOpenMPNormal(ParameterHolder ph): ComputerOpenMP(ph){};
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);
    virtual vector2D computeCandidate(vector2D collisionMatrix);
};

class ComputerOpenMPQuick: public ComputerOpenMP{

public:
    ComputerOpenMPQuick(ParameterHolder ph): ComputerOpenMP(ph){};
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);
};


class ComputerStdThread: public Computer{

public:
    ComputerStdThread(ParameterHolder ph):Computer(ph){};
    virtual vector2D computeHash(vector2D dataset, size_t pNum);
};

class ComputerStdThreadNormal: public ComputerStdThread{

public:
    ComputerStdThreadNormal(ParameterHolder ph): ComputerStdThread(ph){};
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);
    virtual vector2D computeCandidate(vector2D collisionMatrix);
};

class ComputerStdThreadQuick: public ComputerStdThread{

public:
    ComputerStdThreadQuick(ParameterHolder ph): ComputerStdThread(ph){};
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);
};


class ComputerPthread: public Computer{

public:
    ComputerPthread(ParameterHolder ph):Computer(ph){};
    virtual vector2D computeHash(vector2D dataset, size_t pNum);
};

class ComputerPthreadNormal: public ComputerPthread{

public:
    ComputerPthreadNormal(ParameterHolder ph): ComputerPthread(ph){};
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);
    virtual vector2D computeCandidate(vector2D collisionMatrix);
};

class ComputerPthreadQuick: public ComputerPthread{

public:
    ComputerPthreadQuick(ParameterHolder ph): ComputerPthread(ph){};
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);
};






#endif //FASTLSH_COMPUTER_H
