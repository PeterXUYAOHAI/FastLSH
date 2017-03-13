//
// Created by peter on 17-3-13.
//

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


struct args{
    size_t N; //# of vectors in the dataset
    size_t Q; //# of vertors in the queryset
    size_t D; //# of dimensions
    size_t L; //# of group hash
    size_t K; //# the number of hash functions in each group hash
    double W; //bucket width
    size_t T; // threshold
    int computeMode;
    int threadMode;
    bool useHdfs;
    std::string inputPathN;
    std::string inputPathQ;
    std::string outputPath;

};

static std::vector<std::string> argumentReader(char* filePath){
    std::ifstream file;
    file.open(filePath);
    std::string line;
    std::vector<std::string> argus(20);
    int arguCount = 0;
    while(std::getline(file, line)){
        if (line[0] != '#' && !line.empty()){
            argus[arguCount] = line;
            arguCount++;
        }
    }

//    for (int i = 0; i < 13; ++i) {
//        std::cout<<argus[i]<<"\n";
//    }

    return argus;
}

static args arguParser(std::vector<std::string> argus){
    args theArgs;
    theArgs.N =(size_t)std::stoi(argus[0]);
    theArgs.Q =(size_t)std::stoi(argus[1]);
    theArgs.D =(size_t)std::stoi(argus[2]);
    theArgs.L =(size_t)std::stoi(argus[3]);
    theArgs.K =(size_t)std::stoi(argus[4]);
    theArgs.W = std::stod(argus[5]);
    theArgs.T =(size_t)std::stoi(argus[6]);
    theArgs.computeMode = std::stoi(argus[7]);
    theArgs.threadMode= std::stoi(argus[8]);
    if(argus[9]=="Y")
        theArgs.useHdfs = true;
    if(argus[9]=="N")
        theArgs.useHdfs = false;
    theArgs.inputPathN = argus[10];
    theArgs.inputPathQ = argus[11];
    theArgs.outputPath = argus[12];

    return theArgs;
};

static bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}