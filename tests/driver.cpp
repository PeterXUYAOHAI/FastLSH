#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


#include "../include/LSH.h"
#include "helper.cpp"
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
        RUN_ALL_TESTS();
    }
    else if(input!="N"){
        exit(0);
    }
    system("clear");
    std::cout<<"All test passed\n";

    args theArgs;
    if(is_file_exist("./FastLSHargs")){
        std::cout<<"FastLSHargs FOUND, would you like use the parameters in the file? (Y/N)\n";
        std::cin>>input;
        if (input=="Y"){
            std::vector<std::string> argus=  argumentReader("./FastLSHargs");
            theArgs = arguParser(argus);
        }
        else if(input=="N"){
               theArgs=readArguFromConsole();
        }
    }
    else{
        std::cout<<"FastLSHargs NOT FOUND, would you like to input the parameters by yourself? (Y/N)\n";
        std::cin>>input;
        if(input=="Y"){
            theArgs = readArguFromConsole();
        }
        else{
            exit(0);
        }

    }



    LSH mlsh = LSH(theArgs.N, theArgs.Q, theArgs.D, theArgs.L, theArgs.K, theArgs.W, theArgs.T);
//
    mlsh.setComputeMode(theArgs.computeMode);
    mlsh.setThreadMode(theArgs.threadMode);
    mlsh.setUseHdfs(theArgs.useHdfs);
    mlsh.reportStatus();
    std::cout<<"InputN Path "<<theArgs.inputPathN<<"\n";
    std::cout<<"InputQ Path "<<theArgs.inputPathQ<<"\n";
    std::cout<<"Output Path "<<theArgs.outputPath<<"\n";

    std::cout<<"Would you like to continue? (Y/N)";
    std::cin>>input;

    if(input!="Y")
        exit(0);

    system("clear");

    std::cout<<"Loading DataSet.....\n";
    mlsh.loadSetN(theArgs.inputPathN.c_str(),0);
    mlsh.loadSetQ(theArgs.inputPathQ.c_str(),0);

//    mlsh.reportStatus();

    std::cout<<"Calculating.....\n";
    mlsh.getCandidateSet();

    mlsh.reportStatus();

    std::cout<<"Saving.....\n";
    mlsh.saveCandidateSet(theArgs.outputPath.c_str());

    std::cout<<"Done!\n";


}