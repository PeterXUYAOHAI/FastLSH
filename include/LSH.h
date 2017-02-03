//
// Created by peter on 17-2-4.
//

#include <vector>
#include <cstddef>
#include <string>

#ifndef FASTLSH_LSH_H
#define FASTLSH_LSH_H

class LSH{
    public:

        std::vector<std::vector<double>> loadData(std::string filePath, size_t row, size_t col);
        void computeHash();




    private:

        size_t N; //# of vectors in the dataset
        size_t D; //# of dimensions
        size_t L; //# of group hash
        size_t K; //# the number of hash functions in each group hash
        size_t M; //# of dimensions at projection space
        size_t W; //bucket width
        size_t mSize;
        std::vector<double> rawData;
};




#endif //FASTLSH_LSH_H
