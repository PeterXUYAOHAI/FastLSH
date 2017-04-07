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
    Computer.h
    Purpose: This is the header file of Computer classes
    Patterns: strategy, bridge
    Computer is response for all the computation in LSH

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#ifndef FASTLSH_COMPUTER_H
#define FASTLSH_COMPUTER_H

#include <vector>
#include <cstdio>
#include <sstream>
#include "ParameterHolder.h"

typedef std::vector<std::vector<std::vector < double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

/**
 * abstract class of Computer
 */
class Computer{

public:
    /*
     * default constructor
     */
    Computer(){};

    /**
     * constructor
     * @param ph
     */
    Computer(ParameterHolder ph) : ph(ph) {};

    /**
     * abstract function of computeHash, generate hash matrix from dataset
     * @param dataset 2D original dataSet
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    virtual vector2D computeHash(vector2D dataset, size_t pNum)=0;

    /**
     * compute the collision matrix
     * @param hMatrixN N dataset
     * @param hMatrixQ query dataset
     * @return 2D vector<double> of collision value
     */
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ){};

    /**
     * compute the candidate set
     * @param collisionMatrix
     * @return 2D vector of candidate set
     */
    virtual vector2D computeCandidate(vector2D collisionMatrix){};

    /**
     *
     * @param hMatrixN
     * @param hMatrixQ
     * @return 2D vector of candidate set
     */
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ){};

    /**
     * abstract function to print compute mode
     */
    virtual void printComputeMode()=0;

    /**
     * abstract function to print thread mode
     */
    virtual void printThreadMode()=0;

protected:
    ParameterHolder ph; //parameter holder from LSH class
};

/**
 * single Thread computer derive from abstract Computer
 */
class ComputerSingleThread: public Computer{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerSingleThread(ParameterHolder ph):
            Computer(ph){};

    /**
     * compute the hash in singlethread mode
     * @param dataset 2D original dataSet
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    virtual vector2D computeHash(vector2D dataset, size_t pNum);

    /**
     * print the thread mode - singleThread
     */
    virtual void printThreadMode();
};

/**
 * normal mode singleThread computer derive from ComputerSingeThread
 */
class ComputerSingleThreadNormal: public  ComputerSingleThread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerSingleThreadNormal(ParameterHolder ph):
            ComputerSingleThread(ph){};

    /**
     * compute the collision matrix
     * @param hMatrixN
     * @param hMatrixQ
     * @return 2D vector<double> of collision matrix
     */
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * compute the candidate sets in singleThread  Normal Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D collisionMatrix);

    /**
     * print the compute Mode
     */
    virtual void printComputeMode();

};

/**
 * quick mode singleThread computer derive from ComputerSingeThread
 */
class ComputerSingleThreadQuick: public  ComputerSingleThread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerSingleThreadQuick(ParameterHolder ph):
            ComputerSingleThread(ph){};

    /**
      * compute the candidate sets in singleThread Quick Mode
      * @param collisionMatrix
      * @return 2D vector<double> of candidate set
      */
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * print the compute Mode - quick mode
     */
    virtual void printComputeMode();
};


/**
 * openMP computer derived from Computer
 */
class ComputerOpenMP: public Computer{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerOpenMP(ParameterHolder ph):Computer(ph){};

    /**
     * compute the hash in openMP mode
     * @param dataset 2D original dataSet
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    virtual vector2D computeHash(vector2D dataset, size_t pNum);

    /**
     * print the thread mode - openMP
     */
    virtual void printThreadMode();
};

/**
 *  normal mode openMP computer derived from ComputerOpenMP
 */
class ComputerOpenMPNormal: public ComputerOpenMP{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerOpenMPNormal(ParameterHolder ph): ComputerOpenMP(ph){};

    /**
     * compute the candidate sets in singleThread  Normal Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * compute the candidate sets in singleThread  Normal Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D collisionMatrix);

    /**
     * print the compute Mode - normal mode
     */
    virtual void printComputeMode();
};

/**
 * quick mode OpenMP computer derived from ComputerOpenMP
 */
class ComputerOpenMPQuick: public ComputerOpenMP{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerOpenMPQuick(ParameterHolder ph): ComputerOpenMP(ph){};

    /**
     * compute the candidate sets in OpenMP Quick Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * print the compute Mode - quick mode
     */
    virtual void printComputeMode();
};

/**
 * stdThread computer derived from Computer
 */
class ComputerStdThread: public Computer{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerStdThread(ParameterHolder ph):Computer(ph){};

    /**
     * compute the hash in stdThread mode
     * @param dataset 2D original dataSet
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    virtual vector2D computeHash(vector2D dataset, size_t pNum);

    /**
     * print the thread mode - stdThread
     */
    virtual void printThreadMode();

};

/**
 * normal mode stdThread computer derive from ComputerStdThread
 */
class ComputerStdThreadNormal: public ComputerStdThread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerStdThreadNormal(ParameterHolder ph): ComputerStdThread(ph){};

    /**
    * compute the candidate sets in stdThread  Normal Mode
    * @param collisionMatrix
    * @return 2D vector<double> of candidate set
    */
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * compute the candidate sets in stdThread  Normal Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D collisionMatrix);

    /**
     * print the compute Mode - normal mode
     */
    virtual void printComputeMode();
};

/**
 * quick mode stdThread computer derive from ComputerStdThread
 */
class ComputerStdThreadQuick: public ComputerStdThread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerStdThreadQuick(ParameterHolder ph): ComputerStdThread(ph){};

    /**
     * compute the candidate sets in stdThread Quick Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * print the compute Mode - quick mode
     */
    virtual void printComputeMode();

};

/**
 * Pthread computer derived from Computer
 */
class ComputerPthread: public Computer{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerPthread(ParameterHolder ph):Computer(ph){};

    /**
     * compute the hash in Pthread mode
     * @param dataset 2D original dataSet
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    virtual vector2D computeHash(vector2D dataset, size_t pNum);

    /**
     * print the thread mode - Pthread
     */
    virtual void printThreadMode();
};

/**
 * normal mode Pthread computer derive from ComputerPthread
 */
class ComputerPthreadNormal: public ComputerPthread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerPthreadNormal(ParameterHolder ph): ComputerPthread(ph){};

    /**
    * compute the candidate sets in Pthread  Normal Mode
    * @param collisionMatrix
    * @return 2D vector<double> of candidate set
    */
    virtual vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * compute the candidate sets in Pthread  Normal Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D collisionMatrix);

    /**
     * print the compute Mode - normal mode
     */
    virtual void printComputeMode();

};

/**
 * quick mode singleThread computer derive from ComputerPthread
 */
class ComputerPthreadQuick: public ComputerPthread{

public:
    /**
     * constructor
     * @param ph parameterHolder
     */
    ComputerPthreadQuick(ParameterHolder ph): ComputerPthread(ph){};

    /**
     * compute the candidate sets in Pthread Quick Mode
     * @param collisionMatrix
     * @return 2D vector<double> of candidate set
     */
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * print the compute Mode - quick mode
     */
    virtual void printComputeMode();
};

#endif //FASTLSH_COMPUTER_H
