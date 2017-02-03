//
// Created by peter on 17-2-4.
//

#include<vector>
#include <cstddef>

#ifndef FASTLSH_LSH_H
#define FASTLSH_LSH_H

class LSH{
    public:
        


    private:

        size_t N; //# of vectors in the dataset
        size_t D; //# of dimensions
        size_t L; //# of hash tables
        size_t M; //# of dimensions at projection space
        size_t W; //bucket width
        size_t mSize;
        std::vector<int> rawData;

};




#endif //FASTLSH_LSH_H
