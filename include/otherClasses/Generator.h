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
    Generator.h
    Purpose: This is the header file of Generator classes
    Patterns: strategy, bridge
    Generator is response for bridge the LSH class and Computer Class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#ifndef FASTLSH_GENERATOR_H
#define FASTLSH_GENERATOR_H

#include "Computer.h"

/**
 * the generator class which bridge the LSH class and Computer class
 */
class Generator{

public:
    /**
     * constructor
     * @param ph papameterHolder
     */
    Generator(ParameterHolder ph);

    /**
     * change the instance of computer
     * @param multiThreadMode //default 0-singleThread 1-openMP, 2-stdthread 3-pthread
     * @param computeMode //default 0-normal(need more memory)   1-quickMode(need less memory, the collision matrix won't be generated)
     */
    void changeComputer(int multiThreadMode, int computeMode);

    /**
     * call the computer to generate the hashMatrix
     * @param dataset
     * @param pNum number of points
     * @return 2D vector<double> of the hashMatrix
     */
    vector2D generateHash(vector2D dataset, size_t pNum);

    /**
     * call the computer to generate the collision matrix
     * @param hMatrixN hashMatrix N
     * @param hMatrixQ hashMatrix Q
     * @return 2D vector<double> of the collisionMatrix
     */
    vector2D generateCollision(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * call the computer to generate the candidate in normal mode
     * @param collisionMatrix
     * @return 2D vector<double> of the candidate set
     */
    vector2D generateCandidate(vector2D collisionMatrix);

    /**
     * call teh computer to generate the candidate in quick mode
     * @param hMatrixN hashMatrix N
     * @param hMatrixQ hashMatrix Q
     * @return 2D vector<double> of the candidate set
     */
    vector2D generateCandidate(vector2D hMatrixN, vector2D hMatrixQ);

    /**
     * print the compute mode of current computer
     */
    void printComputeMode();

    /**
     * print the thread mode of current computer
     */
    void printThreadMode();

private:

    Computer *theComputer; // the instance of computer class
    ParameterHolder ph; // the parameter holder
};

#endif //FASTLSH_GENERATOR_H
