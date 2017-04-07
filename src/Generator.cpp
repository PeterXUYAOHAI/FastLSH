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
    Generator.cpp
    Purpose: This is the source file of Generator class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/


#include <iostream>
#include "../include/otherClasses/Generator.h"

Generator::Generator(ParameterHolder ph):ph(ph){
    theComputer = new ComputerSingleThreadNormal(ph);
}

void Generator::printComputeMode(){
    theComputer->printComputeMode();
}

void Generator::printThreadMode(){
    theComputer->printThreadMode();
}

void Generator::changeComputer(int threadMode, int computeMode){

    delete theComputer;
    if(computeMode==0)
        switch(threadMode){
            case 0:
                theComputer = new ComputerSingleThreadNormal(ph);
                break;
            case 1:
                theComputer = new ComputerOpenMPNormal(ph);
                break;
            case 2:
                theComputer = new ComputerStdThreadNormal(ph);
                break;
            case 3:
                theComputer = new ComputerPthreadNormal(ph);
                break;
            default:
                std::cout<<"Wrong threadMode Parameter, it must be 0-3\n";
                exit(0);
        }
    else if(computeMode==1) {
        switch (threadMode) {
            case 0:
                theComputer = new ComputerSingleThreadQuick(ph);
                break;
            case 1:
                theComputer = new ComputerOpenMPQuick(ph);
                break;
            case 2:
                theComputer = new ComputerStdThreadQuick(ph);
                break;
            case 3:
                theComputer = new ComputerPthreadQuick(ph);
                break;
            default:
                std::cout << "Wrong threadMode Parameter, it must be 0-3\n";
                exit(0);
        }
    }
    else{
            std::cout<<"Wrong computeMode Parameter, it must be 0-1\n";
            exit(0);
        }
}

vector2D Generator::generateHash(vector2D dataset, size_t pNum){
    return  theComputer->computeHash(dataset,pNum);
}

vector2D Generator::generateCollision(vector2D hMatrixN, vector2D hMatrixQ){
    return theComputer->computeCollision(hMatrixN,hMatrixQ);
}

vector2D Generator::generateCandidate(vector2D collisionMatrix){
    return theComputer->computeCandidate(collisionMatrix);
}

vector2D Generator::generateCandidate(vector2D hMatrixN, vector2D hMatrixQ){
    return theComputer->computeCandidate(hMatrixN,hMatrixQ);
}