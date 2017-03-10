//
// Created by peter on 17-3-10.
//

#ifndef FASTLSH_PARAMETERHOLDER_H
#define FASTLSH_PARAMETERHOLDER_H

#include <cstdio>
#include <vector>


typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

struct ParameterHolder{
    size_t N; //# of vectors in the dataset
    size_t Q; //# of vertors in the queryset
    size_t D; //# of dimensions
    size_t L; //# of group hash
    size_t K; //# the number of hash functions in each group hash
    double W; //bucket width
    size_t T; // threshold
    vector3D randomLine; //collection of randomline for points to project on
    vector1D randomVector; //random values to assist k group of LSH
};


struct DataSetHolder{
    vector2D setN; // original data set of N
    vector2D setQ; // original data set of Q

    //todo this two brings extra cost of memory, may merge to the setN, setQ to save memory
    vector2D setNNorm; // normalized data set of N
    vector2D setQNorm; // normalized data set of Q

    vector2D hashMatrixN; // hashMatrix of N where hash value is stored
    vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored

    vector2D collisionMatrix; //the place to store collision
    vector2D candidateSet; //Qx--, candidate set

};




#endif //FASTLSH_PARAMETERHOLDER_H
