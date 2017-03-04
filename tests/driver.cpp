#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


#include "../include/LSH.h"
#include "../include/hdfs.h"
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


////    //TODO multithread-read
////    //TO-DO in memory data store-memcache, Redis
////    //TODO distrubuted program-hadoop
////    //TODO change generate collision table procedure--no need to store the whole table, as soon as the candidate appear, push back the candidate.
////    //TODO add conservative style -- delete hash as soon as it used
////    //TODO add fed query set style
////    //TODO finish cmd line style
////    //TODO Set similarity style
////
    if(input=="Y") {
        //run the registered tests
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    else{
        exit(0);
    }



    //try stdthread
//    const size_t nthreads = std::thread::hardware_concurrency();
//    {
//        const size_t outloop = 3;
//        const size_t inloop = 5;
//        const size_t nloop = outloop*inloop;
//
//        // Pre loop
//        std::cout<<"parallel ("<<nthreads<<" threads):"<<std::endl;
//        std::vector<std::thread> threads(nthreads);
//        std::mutex critical;
//
//        for(int t = 0;t<nthreads;t++)
//        {
//            threads[t] = std::thread(std::bind(
//                    [&](const int bi, const int ei, const int t)
//                    {
//                        // loop over all items
//                        for(int i = bi;i<ei;i++)
//                        {
//                            // inner loop
//                            {
//                                // (optional) make output critical
//                                std::lock_guard<std::mutex> lock(critical);
//                                std::cout<<i/inloop<<" "<<i%inloop<<std::endl;
//                            }
//                        }
//                    },t*nloop/nthreads,(t+1)==nthreads?nloop:(t+1)*nloop/nthreads,t));
//        }
//
//        std::for_each(threads.begin(),threads.end(),[](std::thread& x){x.join();});
//        // Post loop
//        std::cout<<std::endl;
//    }







//compute hashmatrix
//    vector2D hashmatrix;
//    hashmatrix = mlsh.computeHash(test,200,1.2,1000,57,3);
//    std::cout<<hashmatrix[0][0]<<std::endl;
//    std::cout<<hashmatrix.size()<<std::endl;
//    std::cout<<hashmatrix[0].size()<<std::endl;

}