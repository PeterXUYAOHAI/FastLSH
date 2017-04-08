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
    ParameterHolder.h
    Purpose: This is the header file of ParameterHolder struct
    ParameterHolder is response for holding the LSH parameters to let it pass around during runtime

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#ifndef FASTLSH_PARAMETERHOLDER_H
#define FASTLSH_PARAMETERHOLDER_H

#include <cstdio>
#include <vector>


typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

/**
 * holder of the parameters
 */
struct ParameterHolder{
    size_t N; //# of vectors in the dataset
    size_t Q; //# of vectors in the queryset
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
