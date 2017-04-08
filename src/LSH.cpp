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
    LSH.cpp
    Purpose: This is the source file of LSH class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/
#include "../include/LSH.h"
#include <fstream>
#include <iostream>
#include <random>

LSH::LSH() {
//    theGenerator = new Generator(ph);
//    theFileLoader = new FileLoader();
}

LSH::LSH(size_t N, size_t Q, size_t D, size_t L, size_t K, double W,  size_t T)
{
    ph.N = N;
    ph.D = D;
    ph.L = L;
    ph.K = K;
    ph.W = W;
    ph.Q = Q;
    ph.T = T;

    // do the default setting
    computeMode = 0;
    threadMode = 0;
    ph.randomLine = generateRandomLine();
    ph.randomVector = generateUniformRandomVector(K,W);
    theGenerator = new Generator(ph);
    runID = generateRunId();
    theFileLoader = new LfsLoader();
    std::cout<<"This runId is "<<runID<<std::endl;
}

void LSH::loadSetN(const char* filePath, int fileSize){
    setN = theFileLoader->loadFile(filePath,ph.N,ph.D);
}

void LSH::loadSetQ(const char* filePath, int fileSize){
    setQ = theFileLoader->loadFile(filePath,ph.Q,ph.D);
}


void LSH::saveCandidateSet(const char *filePath){
    //check if the filePath work
    std::ofstream out(filePath);
    if (! out.is_open())
    {
        std::cout << "error opening output file";
        exit (1);
    }

    for (int i = 0; i < candidateSet.size(); ++i) {
        for (int j = 0; j < candidateSet[i].size(); ++j) {
            out << candidateSet[i][j] << ',';
        }
        out<<'\n';
    }
    out.close();
}


vector3D LSH::generateRandomLine(){

    std::random_device rd;
    std::mt19937 gen(rd());
    vector3D randomLine;
    for (int i = 0; i < ph.L; i++) {
        vector2D vK;
        for (int j = 0; j < ph.K; ++j) {
            //generate random according normal distribution
            std::normal_distribution<double> distribution(0.5,0.5);
            std::vector<double> vD;
            for (int k = 0; k < ph.D; ++k) {
                vD.push_back(distribution(gen));
            }
            vK.push_back(vD);
        }
        randomLine.push_back(vK);
    }
    return randomLine;
}


vector1D LSH::generateUniformRandomVector(size_t number, double maxium){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, maxium);

    vector1D UniformRandomVector;
    for (int i = 0; i < number; ++i) {
        UniformRandomVector.push_back(dis(gen));
    }

    return UniformRandomVector;
}

std::string LSH::generateRunId(){
    std::string runId = "";
    time_t t = time(0);
    struct tm * now = localtime( & t );

    runId+=std::to_string(now->tm_year+1900);
    runId+=std::to_string(now->tm_mon);
    runId+=std::to_string(now->tm_mday);
    runId+=std::to_string(now->tm_hour);
    runId+=std::to_string(now->tm_min);
    runId+=std::to_string(now->tm_sec);

    return runId;
}


vector2D LSH::getCollisionMatrix() {
    //check if user have upload dataset
    if(setN.size()==0||setQ.size()==0){
        std::cout<<"you haven't upload setN or setQ\n";
        return collisionMatrix;
    }

    //if it is quick mode then refuse to generate, quick mode cannot generate collisionMatrix
    if(computeMode==1){
        std::cout<<"CollisionMatrix won't be generated in quick mode, Set to Normal Mode and Retry\n";
        return collisionMatrix;
    }

    // if collisionMatrix hasn't been generated then compute the collisionMatrix
    if(collisionMatrix.size()==0) {
        // if hashMatrixes haven't been generated then compute the hashMatrix
        if(hashMatrixN.size()==0||hashMatrixQ.size()==0) {
            // normalize before do the computation
            setQ =normalize(setQ);
            setN =normalize(setN);
            hashMatrixN = theGenerator->generateHash(setN,ph.N);
            hashMatrixQ = theGenerator->generateHash(setQ,ph.Q);
        }
        collisionMatrix = theGenerator->generateCollision(hashMatrixN,hashMatrixQ);
    }
    return collisionMatrix;
}

vector2D LSH::getCandidateSet(){
    //check if user have upload dataset
    if(setN.size()==0||setQ.size()==0){
        std::cout<<"you haven't upload setN or setQ\n";
        return collisionMatrix;
    }
    //use size to check if candidateSet exists, if not generate it
    if(candidateSet.size()==0){
        //if hash matrix not calculated
        if(hashMatrixN.size()==0||hashMatrixQ.size()==0) {
            // normalize before do the computation
            setQ =normalize(setQ);
            setN =normalize(setN);
            hashMatrixN = theGenerator->generateHash(setN,ph.N);
            hashMatrixQ = theGenerator->generateHash(setQ,ph.Q);
        }
        // if it is normal mode, use normal mode workflow
        if(computeMode==0){
            collisionMatrix = theGenerator->generateCollision(hashMatrixN,hashMatrixQ);
            candidateSet = theGenerator->generateCandidate(collisionMatrix);
        }
        // if it is quick mode, use quick mode workflow
        else if(computeMode==1){
            candidateSet = theGenerator->generateCandidate(hashMatrixN,hashMatrixQ);
        }
    }
    return this->candidateSet;
}


void LSH::clearHashMatrix(){
    //this is a trick to release vector memory
    vector2D temp1;
    vector2D temp2;
    hashMatrixQ.swap(temp1);
    hashMatrixN.swap(temp1);
}


void LSH::clearCollisionMatrix(){
    // a trick to release vector memory
    vector2D temp;
    collisionMatrix.swap(temp);
}

void LSH::clearCandidateSet(){
    // a trick to release vector memory
    vector2D temp;
    candidateSet.swap(temp);
}

bool LSH::setThreadMode(int multiMode){
    threadMode = multiMode;
    // let generator recreate computer
    theGenerator->changeComputer(threadMode,computeMode);
    return true;
}

bool LSH::setDefault(){
//    useHdfs = false;
    delete theFileLoader;
    theFileLoader = new LfsLoader();
    threadMode = 0;
    computeMode = 0;
    return true;
}

bool LSH::setUseHdfs(bool useHdfs){
    delete theFileLoader;
    if(useHdfs)
        theFileLoader = new HdfsLoader();
    else
        theFileLoader = new LfsLoader();
}

bool LSH::setComputeMode(int computeMode){
    this->computeMode = computeMode;
    theGenerator->changeComputer(threadMode,computeMode);
    return true;
}

void LSH::clear() {
    clearHashMatrix();
    clearCollisionMatrix();
    clearCandidateSet();
}


void LSH::reportStatus() {

    std::cout<<" __             __   \n"
            "|_  _  _ _|_|  (_ |_|\n"
            "|  (_|_>  |_|____)| |\n";

    std::cout<<"Current RunID: "<<runID<<"\n";
    std::cout<<"Load Mode: ";
    theFileLoader->printLoadMode();
    std::cout<<"\n";
    std::cout<<"Compute Mode: ("<<computeMode<<")";
    theGenerator->printComputeMode();
    std::cout<<"\n";
    std::cout<<"Thread Mode: ("<<threadMode<<")";
    theGenerator->printThreadMode();
    std::cout<<"\n";

    printf("Parameters: \n");
    printf("  --------------------------------\n");
    printf("  |%3s|%10i||%3s|%10i|\n", "N", static_cast<int>(ph.N), "Q", static_cast<int>(ph.Q));
    printf("  --------------------------------\n");
    printf("  |%3s|%10i||%3s|%10i|\n", "D", static_cast<int>(ph.D), "L", static_cast<int>(ph.L));
    printf("  --------------------------------\n");
    printf("  |%3s|%10i||%3s|%10lf|\n", "K",static_cast<int>(ph.K), "W", ph.W);
    printf("  --------------------------------\n");
    printf("  |%3s|%10i||%3s|%10i|\n", "Q", static_cast<int>(ph.Q), "T", static_cast<int>(ph.T));
    printf("  --------------------------------\n");

    std::cout<<"Variables Existence (Y/N): \n";
    std::cout<<"  RandomLine: ";
    if(ph.randomLine.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  RandomVector: ";
    if(ph.randomVector.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  SetN: ";
    if(setN.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  SetQ: ";
    if(setQ.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  hashMatrixN: ";
    if(hashMatrixN.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  hashMatrixQ: ";
    if(hashMatrixQ.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  collisionMatrix: ";
    if(collisionMatrix.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
    std::cout<<"  candidateSet: ";
    if(candidateSet.size()!=0)
        std::cout<<"Y\n";
    else
        std::cout<<"N\n";
}

const vector2D &LSH::getSetN() const {
    return setN;
}

void LSH::setSetN(const vector2D &setN) {
    LSH::setN = setN;
}

const vector2D &LSH::getSetQ() const {
    return setQ;
}

void LSH::setSetQ(const vector2D &setQ) {
    LSH::setQ = setQ;
}

const vector2D &LSH::getHashMatrixN() const {
    return hashMatrixN;
}

const vector2D &LSH::getHashMatrixQ() const {
    return hashMatrixQ;
}


