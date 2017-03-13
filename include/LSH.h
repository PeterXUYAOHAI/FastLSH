//
// Created by peter on 17-2-4.
//

#include <vector>
#include <cstddef>
#include <string>
#include <gtest/gtest_prod.h>
#include "otherClasses/FileLoader.h"
#include "otherClasses/ParameterHolder.h"
#include "otherClasses/Computer.h"
#include "otherClasses/Generator.h"

#ifndef FASTLSH_LSH_H
#define FASTLSH_LSH_H

//typedef to make code less verbose
typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

class LSH{
    public:

    LSH();

    LSH(size_t N, size_t Q, size_t D, size_t L, size_t K, double W,  size_t T);

   // ~LSH();

    void loadSetQ(const char *filePath, int fileSize);

    void loadSetN(const char *filePath, int fileSize);

    void saveCandidateSet(const char *filePath);

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

    void clear();

    int reset();

    const vector2D &getSetN() const;

    void setSetN(const vector2D &setN);

    const vector2D &getSetQ() const;

    void setSetQ(const vector2D &setQ);

    const vector2D &getHashMatrixN() const;

    const vector2D &getHashMatrixQ() const;



private:

    int threadMode; //default 0-singleThread 1-openMP, 2-stdthread 3-pthread
    int computeMode; //default 0-normal((g-collisionmatrix-> candidate)need more memory)   1-quickMode(need less memory, the collision matrix won't be generated
    std::string runID; //runID for recognize this particular run -- mainly for get value from in-memory storage

    FileLoader *theFileLoader;
    Generator *theGenerator;
    ParameterHolder ph;


    vector2D setN; // original data set of N
    vector2D setQ; // original data set of Q

    vector2D hashMatrixN; // hashMatrix of N where hash value is stored
    vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored

    vector2D collisionMatrix; //the place to store collision
    vector2D candidateSet; //Qx--, candidate set

    vector2D normalize(vector2D dataset);

    vector3D generateRandomLine();
    vector1D generateUniformRandomVector(size_t number, double maxium);

    std::string generateRunId();


    FRIEND_TEST(OpenMPTest, hashValueTest);

    FRIEND_TEST(StdthreadTest, hashValueTest);

    FRIEND_TEST(PthreadTest, hashValueTest);

    FRIEND_TEST(MemcachedTest, resultTest);

    FRIEND_TEST(RedisTest, resultTest);

    FRIEND_TEST(SingleThreadTest, collisionMatrixGenTest);

    FRIEND_TEST(SingleThreadTest, clearcollisionMatrixGenTest);

    FRIEND_TEST(MetaTest, linuxReadTest);

    FRIEND_TEST(MetaTest, randGeneTest);

    FRIEND_TEST(MetaTest, clearCollisionMatrixTest);

    FRIEND_TEST(MetaTest, clearCandidateSetTest);

    FRIEND_TEST(MetaTest, clearTest);

    FRIEND_TEST(ComputerTest, singleThreadTest);

    FRIEND_TEST(ComputerTest, openMPTest);

    FRIEND_TEST(ComputerTest, StdThreadTest);

    FRIEND_TEST(ComputerTest, PthreadTest);

    FRIEND_TEST(GeneratorTest, geTest);

    FRIEND_TEST(HdfsTest, readTest);

};


#endif //FASTLSH_LSH_H
