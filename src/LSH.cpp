//
// Created by peter on 17-2-4.
//
#include "../include/LSH.h"
#include "../include/lfsLoader.h"
#include "../include/hdfsLoader.h"
#include <fstream>
#include <iostream>
#include <random>

LSH::LSH() {
}

LSH::LSH(size_t N, size_t D, size_t L, size_t K, double W, size_t Q, size_t T):
        N(N),D(D),L(L),K(K),W(W),Q(Q),T(T) {
    computeMode = 0;
    randomLine = generateRandomLine();
    randomVector = generateUniformRandomVector(K,W);
    runID = generateRunId();
    this->theFileLoader = new LfsLoader();
    std::cout<<"This runId is "<<runID<<std::endl;
}


void LSH::loadSetN(char* filePath, int fileSize){
    setN = theFileLoader->loadFile(filePath,N,D);
}

void LSH::loadSetQ(char* filePath, int fileSize){
    setQ = theFileLoader->loadFile(filePath,Q,D);
}

vector3D LSH::generateRandomLine(){

    std::random_device rd;
    std::mt19937 gen(rd());
    vector3D randomLine;
    for (int i = 0; i < L; i++) {
        vector2D vK;
        for (int j = 0; j < K; ++j) {
            //generate random according normal distribution
            std::normal_distribution<double> distribution(0.5,0.5);
            std::vector<double> vD;
            for (int k = 0; k < D; ++k) {
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


void LSH::generateHashMatrixes(){

    //normalize data
    setQNorm = normalize(setQ);
    setNNorm = normalize(setN);

    //get Hash Matrix
    if (!useMultiThread) {
        hashMatrixN = computeHash(setNNorm, N);
        hashMatrixQ = computeHash(setQNorm, Q);
    }
    else{
        if (multiThreadMode == 0) {
            hashMatrixN = computeHash_openmp(setNNorm, N);
            hashMatrixQ = computeHash_openmp(setQNorm, Q);
        }
        else if(multiThreadMode==1){
            hashMatrixN = computeHash_stdthread(setNNorm, N);
            hashMatrixQ = computeHash_stdthread(setQNorm, Q);
        }
        else if(multiThreadMode ==2){
            hashMatrixN = computeHash_pthread(setNNorm, N);
            hashMatrixQ = computeHash_pthread(setQNorm, Q);
        }
    }

}

void LSH::generateCollisionMatrix(){

    //release the memory of the raw sets(setQ, setN), detail see <Effective STL>
//    vector2D temp1;
//    vector2D temp2;
//    setQ.swap(temp1);
//    setN.swap(temp2);

    //compute collision matrix
    vector2D collisionMatrix;
    if(!useMultiThread)
        collisionMatrix = computeCollision(hashMatrixN, hashMatrixQ);
    else {
        if (multiThreadMode == 0)
            collisionMatrix = computeCollision_openmp(hashMatrixN, hashMatrixQ);
        else if(multiThreadMode ==1)
            collisionMatrix = computeCollision_stdthread(hashMatrixN, hashMatrixQ);
        else if(multiThreadMode ==2)
            collisionMatrix = computeCollision_pthread(hashMatrixN,hashMatrixQ);
    }

    //TODO this need to be modified, duplication exists
    this->collisionMatrix = collisionMatrix;
}


void LSH::generateCandidatesNormal(){
    //TODO here only use the size to check if collisionMatrix exists, may find a better way
    if(collisionMatrix.size()!=Q || collisionMatrix[0].size()!=N)
        generateCollisionMatrix();

    vector2D candidateSet;
    if(!useMultiThread)
        candidateSet = computeCandidateNormal();

    else {
        if (multiThreadMode == 0)
            candidateSet = computeCandidateNormal_openmp();
        else if(multiThreadMode ==1)
            candidateSet = computeCandidateNormal_stdthread();
        else if(multiThreadMode ==2)
            candidateSet = computeCandidateNormal_pthread();
    }

    //TODO this need to be modified, duplication exists
    this->candidateSet = candidateSet;
}


void LSH::generateCandidatesQuick(){

    //compute collision matrix
    vector2D candidateSet;
    if(!useMultiThread)
        candidateSet = computeCandidatesQuick(hashMatrixN, hashMatrixQ, T);
    else {
        if (multiThreadMode == 0)
            candidateSet = computeCandidatesQuick_openmp(hashMatrixN, hashMatrixQ,T);
        else if(multiThreadMode ==1)
            candidateSet = computeCandidatesQuick_stdthread(hashMatrixN, hashMatrixQ,T);
        else if(multiThreadMode ==2)
            candidateSet = computeCandidateQuick_pthread(hashMatrixN,hashMatrixQ,T);
    }

    //TODO this need to be modified, duplication exists
    this->candidateSet = candidateSet;
}


//use timestamp as run id
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

    if(collisionMatrix.size()==0) {
        if (hashMatrixQ.size() == 0 || hashMatrixQ.size() == 0)
            generateHashMatrixes();
        generateCollisionMatrix();
    }
    return collisionMatrix;
}

vector2D LSH::getCandidateSet(){
    //use size to check if candidateSet exists, if not generate it
    if(candidateSet.size()==0){
        //if hash matrix not calculated
        if(hashMatrixN.size()==0||hashMatrixQ.size()==0)
            generateHashMatrixes();
        if(computeMode==0)
            generateCandidatesNormal();
        else if(computeMode==1)
            generateCandidatesQuick();
    }
    return this->candidateSet;
}


void LSH::clearHashMatrix(){
    vector2D temp1;
    vector2D temp2;
    hashMatrixQ.swap(temp1);
    hashMatrixN.swap(temp1);
}


void LSH::clearCollisionMatrix(){
    vector2D temp;
    collisionMatrix.swap(temp);
}

void LSH::clearCandidateSet(){
    vector2D temp;
    candidateSet.swap(temp);
}

bool LSH::setMultiThreadMode(int multiMode){

    useMultiThread = true;

    multiThreadMode = multiMode;
}

bool LSH::setDefault(){
//    useHdfs = false;
    delete theFileLoader;
    theFileLoader = new LfsLoader();

    useMultiThread = false;
    multiThreadMode = 0;
    computeMode = 0;
    return true;
}

bool LSH::setUseHdfs(bool useHdfs){
    delete theFileLoader;
    if(useHdfs)
        theFileLoader = new HdfsLoader();
    else
        theFileLoader = new LfsLoader();
//    this->useHdfs = useHdfs;
//    return this->useHdfs;
}

bool LSH::setUseMultiThread(bool useMultiThread) {
    this->useMultiThread = useMultiThread;
    multiThreadMode = 0; //set as default mode - openMP
    return this->useMultiThread;
}

bool LSH::setComputeMode(int computeMode){
    this->computeMode = computeMode;
    return true;
}





