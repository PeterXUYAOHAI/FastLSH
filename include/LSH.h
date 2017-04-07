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
    LSH.h
    Purpose: This is the header file of LSH class

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

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

    /**
     * default constructor
     */
    LSH();

    /**
     * constructor
     * @param N # of vectors in the dataset
     * @param Q # of vertors in the queryset
     * @param D # of dimensions
     * @param L # of group hash
     * @param K # the number of hash functions in each group hash
     * @param W bucket width
     * @param T threshold
     */
    LSH(size_t N, size_t Q, size_t D, size_t L, size_t K, double W,  size_t T);

    /**
     * load the query set into the object
     * @param filePath string or char*
     * @param fileSize the fileSize, this parameter was for HDFS read when the file is too large. Put 0 for default setting
     */
    void loadSetQ(const char *filePath, int fileSize);

    /**
     * load the N(training) set into the object
     * @param filePath string or char*
     * @param fileSize the fileSize, this parameter was for HDFS read when the file is too large. Put 0 for default setting
     */
    void loadSetN(const char *filePath, int fileSize);

    /**
     * save the candidate to the disk
     * @param filePath string or char*
     */
    void saveCandidateSet(const char *filePath);

    /**
     * read and parse stored hash N from Memcached
     * @param server the server ip address
     * @param port the port number of the memcached service
     * @param runId the runId of the stored hash N
     */
    void readHashNFromMemcached(const char *server, unsigned short port, std::string runId);

    /**
     * read and parse stored hash N from Redis
     * @param server the server ip address
     * @param port the port number of the Redis service
     * @param runId the runId of the stored hash N
     */
    int readHashNFromRedis(const char *server, unsigned short port, std::string srunId);

    /**
     * save hash N to memcached
     * @param server the memcached server IP
     * @param port the memcached port number
     * @param exp how long it gonna stay in memory (0 for forever)
     * @return 0-not success 1-success
     */
    int saveHashNToMemcached(const char *server, unsigned short port, time_t exp);

    /**
     * save hash N to Redis
     * @param server the Redis server IP
     * @param port the Redis port number
     * @return 0-not success 1-success
     */
    int saveHashNToRedis(const char *server, unsigned short port);

    /**
     * delete the hashMatrixes, release the memory
     */
    void clearHashMatrix();

    /**
     * delete the hashMatrix, collisionMatrix and candidateSet, release the memory
     */
    void clear();

    /**
     * delete the collisionMatrix, release the memory
     */
    void clearCollisionMatrix();

    /**
     * delete the candidateSet, release the memory
     */
    void clearCandidateSet();

    /**
     * print the status(settings, calculation step)
     */
    void reportStatus();

    /**
     * calculate and return the collisionMatrix; in quick mode won't work; won't recalculate if collisionMatrix already exist.
     * @return
     */
    vector2D getCollisionMatrix();

    /**
     * calculate and return the candidateSet; won't recalculate if candidateSet already exist
     * @return
     */
    vector2D getCandidateSet();

    /**
     * set Hdfs read setting
     * @param useHdfs 0-use LFS 1- use HDFS
     * @return 0-not success 1- success
     */
    bool setUseHdfs(bool useHdfs);

    /**
     * return the setN
     * @return
     */
    const vector2D &getSetN() const;

    /**
     * return the setQ
     * @return
     */
    const vector2D &getSetQ() const;

    /**
     * return the hashMatrixN
     * @return
     */
    const vector2D &getHashMatrixN() const;

    /**
     * return the hashMatrixQ
     * @return
     */
    const vector2D &getHashMatrixQ() const;

    /**
     * set thread mode
     * @param threadMode default 0-singleThread, 1-openMP, 2-stdthread, 3-pthread
     * @return 0-not success 1- success
     */
    bool setThreadMode(int threadMode);

    /**
     * set compute Mode
     * @param computeMode default 0-normal(need more memory)  1-quickMode(less memory, skip collisionMatrix)
     * @return 0- not success 1- success
     */
    bool setComputeMode(int computeMode);

    /**
     * import the setN, it should be a 2D vector<double>
     * @param setN
     */
    void setSetN(const vector2D &setN);

    /**
     * import the setQ, it should be a 2D vector<double>
     * @param setQ
     */
    void setSetQ(const vector2D &setQ);

    /**
     * change all the setting to default, singleThread, LFS, Normal Mode
     * @return
     */
    bool setDefault();



private:

    int threadMode; //default 0-singleThread 1-openMP, 2-stdthread 3-pthread
    int computeMode; //default 0-normal((g-collisionmatrix-> candidate)need more memory)   1-quickMode(need less memory, the collision matrix won't be generated
    std::string runID; //runID for recognize this particular run --for read value from in-memory storage

    FileLoader *theFileLoader; //fileLoader to for file loading
    Generator *theGenerator;  //response for computing
    ParameterHolder ph; //hold the parameters


    vector2D setN; // original data set of N
    vector2D setQ; // original data set of Q

    vector2D hashMatrixN; // hashMatrix of N where hash value is stored
    vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored

    vector2D collisionMatrix; //the place to store collision
    vector2D candidateSet; //candidate set

    /**
     *this normalization is do normalize along the column on 2d dataset. the algorithm may have chance to be optimized
     * @param dataset 2d list of double
     * @return normalized 2d list of double
     */
    vector2D normalize(vector2D dataset);

    /**
     * This function generates the random projected lines, it uses normal distribution
     * @return the 3D vector of randomLine generated
     */
    vector3D generateRandomLine();

    /**
     * This funciton generates k number of random vectors, it uses uniform random generator
     * @param number number of randomNumber, usually = K
     * @param maxium the max value for the random generation
     * @return 1D vector of random numbers
     */
    vector1D generateUniformRandomVector(size_t number, double maxium);

    /**
     * generate the RunId for this object, can be used for retrieving data from in-memory storage
     * @return the string of the RunID, it is derived from the runtime timestamp
     */
    std::string generateRunId();


    // declare the friend test functions for google test to test the private functions
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
