//
// Created by peter on 17-2-4.
//

#include <vector>
#include <cstddef>
#include <string>
#include <gtest/gtest_prod.h>
#include "FileLoader.h"
#include "ParameterHolder.h"
#include "Computer.h"
#include "Generator.h"

#ifndef FASTLSH_LSH_H
#define FASTLSH_LSH_H

//typedef to make code less verbose
typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

class LSH{
public:

    LSH();

    LSH(size_t N, size_t D, size_t L, size_t K, double W, size_t Q, size_t T);

    // ~LSH();

    void loadSetQ(char *filePath, int fileSize);

    void loadSetN(char *filePath, int fileSize);

    bool setUseHdfs(bool useHdfs);

    bool setThreadMode(int threadMode); //default 0-singleThread 1-openMP, 2-stdthread 3-pthread

    bool setDefault();

    int saveHashNToMemc(const char *server, unsigned short port, time_t exp);

    void readHashNFromMemc(const char *server, unsigned short port, std::string runId);

    int saveHashNToRedis(const char *server, unsigned short port);

    int readHashNFromRedis(const char *server, unsigned short port, std::string srunId);

    void clearHashMatrix();

    void clearCollisionMatrix();

    void clearCandidateSet();

    vector2D getCollisionMatrix();

    vector2D getCandidateSet();

    bool setComputeMode(int computeMode);

    void reportStatus();

    int clear();

    int reset();



private:

//    size_t N; //# of vectors in the dataset
//    size_t Q; //# of vertors in the queryset
//    size_t D; //# of dimensions
//    size_t L; //# of group hash
//    size_t K; //# the number of hash functions in each group hash
//    double W; //bucket width
//    size_t T; // threshold

    int threadMode; //default 0-singleThread 1-openMP, 2-stdthread 3-pthread
    int computeMode; //default 0-normal((g-collisionmatrix-> candidate)need more memory)   1-quickMode(need less memory, the collision matrix won't be generated
    std::string runID; //runID for recognize this particular run -- mainly for get value from in-memory storage

    FileLoader *theFileLoader;
    ParameterHolder ph;
    DataSetHolder dh;
    Generator *theGenerator;

    vector3D randomLine; //collection of randomline for points to project on
    vector1D randomVector; //random values to assist k group of LSH
    vector2D setN; // original data set of N
    vector2D setQ; // original data set of Q

    //todo this two brings extra cost of memory, may merge to the setN, setQ to save memory
    vector2D setNNorm; // normalized data set of N
    vector2D setQNorm; // normalized data set of Q

    vector2D hashMatrixN; // hashMatrix of N where hash value is stored
    vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored

    vector2D collisionMatrix; //the place to store collision
    vector2D candidateSet; //Qx--, candidate set




    vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    vector3D generateRandomLine();

    vector1D generateUniformRandomVector(size_t number, double maxium);

    vector2D computeHash(vector2D dataset, size_t pNum);

    vector2D computeHash_openmp(vector2D dataset, size_t pointNum);

    vector2D computeCollision_openmp(vector2D hMatrixN, vector2D hMatrixQ);

    vector2D computeHash_stdthread(vector2D dataset, size_t pointNum);

    vector2D computeCollision_stdthread(vector2D hMatrixN, vector2D hMatrixQ);

    vector2D computeHash_pthread(vector2D dataset, size_t pointNum);

    vector2D computeCollision_pthread(vector2D hMatrixN, vector2D hMatrixQ);

    void generateCollisionMatrix();

    friend void *computeHashPthreadFuc(void *loopPara);

    friend void *computeCollisionPthreadFuc(void *loopPara);

    friend void* computeCandidateNormalPthreadFuc(void *loopPara);

    friend void* computeCandidatesQuickPthreadFuc(void *loopPara);

    std::string generateRunId();

    vector2D normalize(vector2D dataset);

    vector2D computeCandidatesQuick(vector2D hMatrixN, vector2D hMatrixQ, size_t T);

    vector2D computeCandidateNormal();

    void generateHashMatrixes();

    vector2D computeCandidateNormal_openmp();

    void generateCandidatesNormal();

    void generateCandidatesQuick();

    vector2D computeCandidatesQuick_openmp(vector2D hMatrixN, vector2D hMatrixQ, size_t T);

    vector2D computeCandidateNormal_stdthread();

    vector2D computeCandidatesQuick_stdthread(vector2D hMatrixN, vector2D hMatrixQ, size_t T);

    vector2D computeCandidateNormal_pthread();

    vector2D computeCandidateQuick_pthread(vector2D hMatrixN, vector2D hMatrixQ, size_t T);

    FRIEND_TEST(openMPTest, hashValueTest);

    FRIEND_TEST(hdfsTest, readTest);

    FRIEND_TEST(metaTest, linuxReadTest);

    FRIEND_TEST(metaTest, randGeneTest);

    FRIEND_TEST(stdthreadTest, hashValueTest);

    FRIEND_TEST(pthreadTest, hashValueTest);

    FRIEND_TEST(memcachedTest, resultTest);

    FRIEND_TEST(redisTest, resultTest);

    FRIEND_TEST(singleThreadTest, collisionMatrixGenTest);

    FRIEND_TEST(singleThreadTest, clearcollisionMatrixGenTest);

    FRIEND_TEST(metaTest, clearCollisionMatrixTest);

    FRIEND_TEST(metaTest, clearCandidateSetTest);

    FRIEND_TEST(computerTest, singleThreadTest);

    FRIEND_TEST(computerTest, openMPTest);

    FRIEND_TEST(computerTest, StdThreadTest);

    FRIEND_TEST(computerTest, PthreadTest);

    FRIEND_TEST(GeneratorTest, geTest);

};


#endif //FASTLSH_LSH_H
