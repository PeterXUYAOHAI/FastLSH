//
// Created by peter on 17-3-10.
//

#include <iostream>
#include "../include/otherClasses/Generator.h"



Generator::Generator(ParameterHolder ph):ph(ph){
    theComputer = new ComputerSingleThreadNormal(ph);
}
//
//Generator::~Generator() {
//   // delete theComputer;
//}

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