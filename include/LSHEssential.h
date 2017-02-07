//
// Created by peter on 17-2-4.
//

#include <vector>
#include <cstddef>
#include <string>

#ifndef FASTLSH_LSH_H
#define FASTLSH_LSH_H

//typedef to make code less verbose
typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

class LSH{
    public:

    LSH();

    LSH(size_t N, size_t D, size_t L, size_t K, double W, size_t Q);

    void loadSetQ(char *filePath, int fileSize);

    void loadSetN(char *filePath, int fileSize);

    bool setUseHdfs(bool useHdfs);

    bool setUseMultiThread(bool useMultiThread);

    vector2D getCollisionMatrix();

    void reportStatus();

private:

    size_t N; //# of vectors in the dataset
    size_t Q; //# of vertors in the queryset
    size_t D; //# of dimensions
    size_t L; //# of group hash
    size_t K; //# the number of hash functions in each group hash
    size_t M; //# of dimensions at projection space
    double W; //bucket width
    bool useHdfs; //whether use hdfs flag
    int NfileSize; //N file binary size, for hdfs use
    int QfileSize; //Q file binary size, for hdfs use
    bool useMultiThread;

    vector3D randomLine; //collection of randomline for points to project on
    vector1D randomVector; //random values to assist k group of LSH
    vector2D setN; // original data set of N
    vector2D setQ; // original data set of Q
    vector2D hashMatrixN; // hashMatrix of N where hash value is stored
    vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored


    vector2D computeHash(vector2D dataset);

    vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);

    vector3D generateRandomLine();

    vector1D generateUniformRandomVector(size_t number, double maxium);

    vector2D loadDataFromHDFS(char *filePath, int row, int col, int fileSize);

    vector2D loadDataFromLinuxSystem(char *filePath, size_t row, size_t col);

    int clear();

    int reset();

    vector2D computeHash(vector2D dataset, size_t pNum);

    vector2D computeHash_openmp(vector2D dataset, size_t pointNum);

    vector2D computeCollision_openmp(vector2D hMatrixN, vector2D hMatrixQ);
};


#endif //FASTLSH_LSH_H
