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
    driver.cpp
    Purpose: This file is to main function file to call google test

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/


#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "../include/LSH.h"
#include "helper.cpp"
#include <gtest/gtest.h>
#include <thread>
#include <mutex>

#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>

int main (int argc, char **argv){
    //http://www.kammerl.de/ascii/AsciiSignature.php
    //display logo
    std::cout<<" _______    ___           _______.___________. __          _______. __    __ \n"
            "|   ____|  /   \\         /       |           ||  |        /       ||  |  |  |\n"
            "|  |__    /  ^  \\       |   (----`---|  |----`|  |       |   (----`|  |__|  |\n"
            "|   __|  /  /_\\  \\       \\   \\       |  |     |  |        \\   \\    |   __   |\n"
            "|  |    /  _____  \\  .----)   |      |  |     |  `----.----)   |   |  |  |  |\n"
            "|__|   /__/     \\__\\ |_______/       |__|     |_______|_______/    |__|  |__|\n";

    std::cout<<"Would you like to run the test(Y/N)\n";
    std::string input;
    std::cin>>input;

    if(input=="Y") {
        //run the registered tests
        ::testing::InitGoogleTest(&argc, argv);
        RUN_ALL_TESTS();
        //        ::testing::GTEST_FLAG(filter) = "ComputerTest*";
        //        ::testing::GTEST_FLAG(filter) = "GeneratorTest*";
    }
    else if(input!="N"){
        exit(0);
    }
    system("clear");
    std::cout<<"All test passed\n";


    //FastLSH-ESS content, read config files and run the engine.
    args theArgs;
    //read in files
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

    std::vector<std::string> argus=  argumentReader("./FastLSHargs");

    //start basic LSH flow according to the input parameters
    LSH mlsh = LSH(theArgs.N, theArgs.Q, theArgs.D, theArgs.L, theArgs.K, theArgs.W, theArgs.T);
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

    mlsh.reportStatus();

    std::cout<<"Calculating.....\n";
    mlsh.getCandidateSet();

    mlsh.reportStatus();

    std::cout<<"Saving.....\n";
    mlsh.saveCandidateSet(theArgs.outputPath.c_str());

    std::cout<<"Done!\n";

}