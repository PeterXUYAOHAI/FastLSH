//
// Created by peter on 17-2-7.
//

#include "../../../include/LSH.h"
#include <thread>
#include <random>
#include <iostream>
#include <algorithm>


vector2D LSH::computeHash_stdthread(vector2D dataset, size_t pointNum){
    vector2D hashMatrix;

    for (int i = 0; i < pointNum; ++i) {
        vector1D vL(L,0);
        hashMatrix.push_back(vL);
    }

    // it is a implementation of nested loop to stdthread
            const size_t nthreads = std::thread::hardware_concurrency();
            const size_t outloop = pointNum;
            const size_t inloop = L;
            const size_t nloop = outloop*inloop;


            std::vector<std::thread> threads(nthreads);
            for(int t = 0;t<nthreads;t++)
            {
                threads[t] = std::thread(std::bind(
                        [&](const int bi, const int ei, const int t)
                        {
                            // loop over all items
                            for(int i = bi;i<ei;i++)
                            {
                                // inner loop
                                {
                                    int n = i/inloop; //retrive outer loop index
                                    int l = i%inloop; //retrive inner loop index
                                    // (optional) make output critical
                                    double hashFinalValue = 0;
                                    //loop through the inner of a hash function group
                                    for (int k = 0; k < K; ++k) {
                                        double dTemp = 0;
                                        //loop through all the dimensions
                                        for (int d = 0; d < D; ++d) {
                                            //vector(math) multiply to make projection
                                            dTemp += dataset[n][d]*randomLine[l][k][d];
                                        }
                                        //assign hashvalue into bucket
                                        double hashvalue = floor(dTemp/W);
                                        //merge hash group results **see documentation
                                        hashFinalValue = hashvalue*randomVector[k] + hashFinalValue;
                                    }
                                    hashMatrix[n][l] = hashFinalValue;

                                }
                            }
                        },t*nloop/nthreads,(t+1)==nthreads?nloop:(t+1)*nloop/nthreads,t));
            }

            //lambda to start threads
            std::for_each(threads.begin(),threads.end(),[](std::thread& x){x.join();});

    return hashMatrix;
}







vector2D LSH::computeCollision_stdthread(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D collisionMatrix;
    for (int i = 0; i < Q ; ++i) {
        vector1D singleLine(N, 0);
        collisionMatrix.push_back(singleLine);
    }

    const size_t nthreads = std::thread::hardware_concurrency();
    const size_t outloop = Q;
    const size_t inloop = N;
    const size_t nloop = outloop*inloop;
    std::vector<std::thread> threads(nthreads);
    for(int t = 0;t<nthreads;t++)
    {
        threads[t] = std::thread(std::bind(
                [&](const int bi, const int ei, const int t)
                {
                    // loop over all items
                    for(int i = bi;i<ei;i++)
                    {
                        // inner loop
                        {
                            // (optional) make output critical
                            int q = i/inloop;
                            int n = i%inloop;
                            for (int hash_id = 0; hash_id < L; ++hash_id) {
                                if (hMatrixN[n][hash_id] == hMatrixQ[q][hash_id])
                                    collisionMatrix[q][n]++;
                            }


                        }
                    }
                },t*nloop/nthreads,(t+1)==nthreads?nloop:(t+1)*nloop/nthreads,t));
    }
    std::for_each(threads.begin(),threads.end(),[](std::thread& x){x.join();});
    return collisionMatrix;
}




//
////try stdthread
//const size_t nthreads = std::thread::hardware_concurrency();
//{
//const size_t outloop = 3;
//const size_t inloop = 4;
//const size_t nloop = outloop*inloop;
//
//// Pre loop
//std::cout<<"parallel ("<<nthreads<<" threads):"<<std::endl;
//std::vector<std::thread> threads(nthreads);
//std::mutex critical;
//
//for(int t = 0;t<nthreads;t++)
//{
//threads[t] = std::thread(std::bind(
//        [&](const int bi, const int ei, const int t)
//        {
//            // loop over all items
//            for(int i = bi;i<ei;i++)
//            {
//                // inner loop
//                {
//                    // (optional) make output critical
//                    std::lock_guard<std::mutex> lock(critical);
//                    std::cout<<i/inloop<<" "<<i%inloop<<std::endl;
//                }
//            }
//        },t*nloop/nthreads,(t+1)==nthreads?nloop:(t+1)*nloop/nthreads,t));
//}
//
//std::for_each(threads.begin(),threads.end(),[](std::thread& x){x.join();});
//// Post loop
//std::cout<<std::endl;
//}
//
//
//
//
//
//