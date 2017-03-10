#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


#include "../include/LSH.h"
#include "hdfs.h"
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <chrono>

#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

int main (int argc, char **argv){

//    LSH mlsh = LSH(100000, 57, 200, 1, 1.2, 100, 100);
//    std::chrono::high_resolution_clock::time_point t1;
//    std::chrono::high_resolution_clock::time_point t2;
//
//    mlsh.loadSetN("/home/mpiuser/dataset100000NoIndex.csv",0);
//    mlsh.loadSetQ("/home/mpiuser/dataset100NoIndex.csv", 0);
//
//    t1 = now();
//    mlsh.getCandidateSet();
//    t2 = now();
//    auto duration = dcast( t2 - t1 ).count();
//    std::cout <<duration << " μs to calculate normal mode\n";
//
//    mlsh.clearCandidateSet();
//    mlsh.clearCollisionMatrix();
//    mlsh.clearHashMatrix();
//
//    mlsh.setUseMultiThread(true);
//    mlsh.setComputeMode(1);
//
//    t1 = now();
//    mlsh.getCandidateSet();
//    t2 = now();
//
//    duration = dcast( t2 - t1 ).count();
//    std::cout <<duration << " μs to to calculate quick mode\n";

    //http://www.kammerl.de/ascii/AsciiSignature.php
    std::cout<<" _______    ___           _______.___________. __          _______. __    __ \n"
            "|   ____|  /   \\         /       |           ||  |        /       ||  |  |  |\n"
            "|  |__    /  ^  \\       |   (----`---|  |----`|  |       |   (----`|  |__|  |\n"
            "|   __|  /  /_\\  \\       \\   \\       |  |     |  |        \\   \\    |   __   |\n"
            "|  |    /  _____  \\  .----)   |      |  |     |  `----.----)   |   |  |  |  |\n"
            "|__|   /__/     \\__\\ |_______/       |__|     |_______|_______/    |__|  |__|\n";

    std::cout<<"Would you like to run the test(Y/N)\n";

    std::string input;
    std::cin>>input;

////    //TODO add fed query set style
////    //TODO finish cmd line style
    if(input=="Y") {
        //run the registered tests
        ::testing::InitGoogleTest(&argc, argv);
//        ::testing::GTEST_FLAG(filter) = "ComputerTest*";
//        ::testing::GTEST_FLAG(filter) = "GeneratorTest*";
        return RUN_ALL_TESTS();
    }
    else{
        exit(0);
    }


}