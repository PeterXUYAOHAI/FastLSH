//
// Created by peter on 17-2-4.
//
#include "../include/LSH.h"
#include <fstream>
#include <iostream>
#include <random>

LSH::LSH() {
}

LSH::LSH(size_t N, size_t D, size_t L, size_t K, double W, size_t Q, size_t T) {
    this->N = N;
    this->D = D;
    this->L = L;
    this->K = K;
    this->W = W;
    this->Q = Q;
    this->T = T;
    useHdfs = false;
    randomLine = generateRandomLine();
    randomVector = generateUniformRandomVector(K,W);
    runID = generateRunId();
    std::cout<<"This runId is "<<runID<<std::endl;
}


void LSH::loadSetN(char* filePath, int fileSize){
    NfileSize = fileSize;
    if(!useHdfs) {
        setN = loadDataFromLinuxSystem(filePath, N, D);
    }
    else {
        setN = loadDataFromHDFS(filePath, N, D, NfileSize);
    }

}

void LSH::loadSetQ(char* filePath, int fileSize){
    QfileSize = fileSize;
    if(!useHdfs)
        setQ = loadDataFromLinuxSystem(filePath, Q, D);
    else
        setQ = loadDataFromHDFS(filePath, Q, D, QfileSize);
}

bool LSH::setUseHdfs(bool useHdfs){
    this->useHdfs = useHdfs;
    return this->useHdfs;
}

bool LSH::setUseMultiThread(bool useMultiThread) {
    this->useMultiThread = useMultiThread;
    multiThreadMode = 0; //set as default mode - openMP
    return this->useMultiThread;
}

vector2D LSH::loadDataFromLinuxSystem(char* filePath, size_t row, size_t col) {
    std::ifstream file;// declare file stream:
    file.open(filePath);
    vector2D data;
    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }
    std::string value;
    std::string num;
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(file, value, ',');
            else
                getline(file,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    file.close();
    return data;
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


vector2D LSH::getCollisionMatrix() {

    if(collisionMatrix.size()==0)
        generateCollisionMatrix();

    return collisionMatrix;
}


void LSH::generateCollisionMatrix(){

    //normalize data
    setQNorm = normalize(setQ);
    setNNorm = normalize(setN);

    //release the memory of the raw sets(setQ, setN), detail see <Effective STL>
//    vector2D temp1;
//    vector2D temp2;
//    setQ.swap(temp1);
//    setN.swap(temp2);

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
//    //release the memory of the hashMatrixs, detail see <Effective STL>
//    vector2D temp3;
//    vector2D temp4;
//    hashMatrixN.swap(temp3);
//    hashMatrixQ.swap(temp4);

    //TODO this need to be modified, duplication exists
    this->collisionMatrix = collisionMatrix;
}


void LSH::clearCollisionMatrix(){
    vector2D temp;
    collisionMatrix.swap(temp);
}



bool LSH::setMultiThreadMode(int multiMode){
    multiThreadMode = multiMode;
}


bool LSH::setDefault(){
    useHdfs = false;
    useMultiThread = false;
    multiThreadMode = 0;

    return true;
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


vector2D LSH::generateCandidateSet(){
    vector2D candidateSet;

    for (int i = 0; i < Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

    for (int i = 0; i < Q; ++i) {
        vector1D candidates;
        for (int j = 0; j < N; ++j) {
            if(collisionMatrix[i][j]>=T)
                candidates.push_back((double &&) j);
        }
        candidateSet[i] = candidates;
    }

    return candidateSet;
}





