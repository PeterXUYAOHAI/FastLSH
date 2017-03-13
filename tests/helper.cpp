//
// Created by peter on 17-3-13.
//

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


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

static args readArguFromConsole(){
    args theArgs;
    std::string input;
    std::cout<<"Please input the following Parameters\n";


    std::cout<< "N<int>: number of points(lines,rows) in the dataset [default=1000]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.N;
    }
    else{
        theArgs.N = 1;
    }

    std::cout<< "Q<int>: number of points(lines,rows) in the queryset [default=1000]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.Q;
    }
    else{
        theArgs.Q = 1000;
    }

    std::cout<< "D<int>: number of dimensions (columns) [default=56]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.D;
    }
    else{
        theArgs.D = 56;
    }

    std::cout<< "L<int>: number of group hash [default=200]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.L;
    }
    else{
        theArgs.L = 200;
    }

    std::cout<< "K<int>: number of hash functions in each group hash [default=1]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.K;
    }
    else{
        theArgs.K = 1;
    }


    std::cout<< "W<double>: bucket width [default=1.2]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.W;
    }
    else{
        theArgs.W = 1.2;
    }

    std::cout<< "T<int>: threshold [default=100]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.T;
    }
    else{
        theArgs.T = 1;
    }

    std::cout<< "Compute Mode(0/1): 0-normal((g-collisionmatrix-> candidate)need more memory) "
            " 1-quickMode(need less memory, the collision matrix won't be generated [default=0]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.computeMode;
    }
    else{
        theArgs.computeMode = 0;
    }

    std::cout<< "Thread Mode (0-3): 0-singleThread 1-openMP, 2-stdthread 3-pthread [default=0]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.threadMode;
    }
    else{
        theArgs.threadMode = 0;
    }

    std::cout<< "useHDFS (Y/N): Whether to load from HDFS [default=N]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.useHdfs;
    }
    else{
        theArgs.useHdfs = false;
    }

    std::cout<< "Input Path for datasetN:  the file path to load datasetN from, "
            "if use HDFS, use the HDFS path. [default=../tests/dataset/dataset1000NoIndex.csv]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.inputPathN;
    }
    else{
        theArgs.inputPathN = "../tests/dataset/dataset1000NoIndex.csv";
    }

    std::cout<< "Input Path for datasetQ:  the file path to load datasetQ from, if use HDFS, "
            "use the HDFS path. [default=../tests/dataset/dataset1000NoIndex.csv]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.inputPathQ;
    }
    else{
        theArgs.inputPathQ = "../tests/dataset/dataset1000NoIndex.csv";
    }

    std::cout<< "Output Path: the file path to save the candidateSet to [default=1000]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> theArgs.outputPath;
    }
    else{
        theArgs.outputPath = "../tests/dataset/candidate.csv";
    }

    return theArgs;
}


static bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}