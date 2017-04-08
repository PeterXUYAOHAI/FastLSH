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
    PthreadComputer.cpp
    Purpose: This is the source file for pthread computing functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include <pthread.h>
#include <random>
#include <iostream>
#include <algorithm>

//assign different libraries when in different system
#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#include "../../include/otherClasses/Computer.h"

#endif

//typedef to make code less verbose
typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

/**
 * get the number of cores in the system
 * @return
 */
int getNumOfCoresN() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

/**
 * struct to hold parameters allows them to pass around threads
 */
struct nLoopHashParaFN{
    int bi; //loop begin index
    int ei; // loop end index
    int inloop; // the inner loop count, for reconstruct 2d loop
    ParameterHolder *ph; // parameterHolder
    vector2D* hashMatrix;
    vector2D* dataset;
};

/**
 * struct to hold parameters allows them to pass around threads
 */
struct nLoopColliParaN{
    int bi; //loop begin index
    int ei; // loop end index
    int inloop;// the inner loop count, for reconstruct 2d loop
    ParameterHolder *ph; // parameterHolder
    vector2D* collisionMatrix;
    vector2D* hMatrixQ;
    vector2D* hMatrixN;
};

/**
 * struct to hold parameters allows them to pass around threads
 */
struct nLoopCandidateParaN{
    int bi; //loop begin index
    int ei; // loop end index
    ParameterHolder *ph; // parameterHolder
    vector2D* hMatrixQ;
    vector2D* hMatrixN;
    vector2D *collisionMatrix;
    vector2D *candidateSet;

};

/**
 * the thread function for computing hash value
 * @param loopPara struct nLoopHashParaFN
 * @return
 */
void* computeHashPthreadFucN(void *loopPara)
{
    // retrieve the parameters
    struct nLoopHashParaFN *my_data;
    int bi,ei,inloop;
    ParameterHolder *ph;
    my_data = (nLoopHashParaFN *) loopPara;
    bi = my_data->bi;
    ei = my_data->ei;
    inloop = my_data->inloop;
    ph = my_data->ph;

    for(int i = bi;i<ei;i++)
    {
        // inner loop
        {
            int n = i/inloop; //retrive outer loop index
            int l = i%inloop; //retrive inner loop index

            double hashFinalValue = 0;
            //loop through the inner of a hash function group
            for (int k = 0; k < ph->K; ++k) {
                double dTemp = 0;
                //loop through all the dimensions
                for (int d = 0; d < ph->D; ++d) {
                    //vector(math) multiply to make projection
                    dTemp += (*(my_data->dataset))[n][d]*ph->randomLine[l][k][d];
                }
                //assign hashvalue into bucket
                double hashvalue = floor(dTemp/ph->W);
                //merge hash group results **see documentation
                hashFinalValue = hashvalue*ph->randomVector[k] + hashFinalValue;
            }
            //put value to the matrix
            (*(my_data->hashMatrix))[n][l] = hashFinalValue;
        }
    }
    return NULL;
}

/**
 * the thread function to compute collision matrix
 * @param loopPara struct nLoopColliParaN
 * @return
 */
void* computeCollisionPthreadFucN(void *loopPara)
{
    //retrieve parameters
    struct nLoopColliParaN *my_data;
    int bi,ei,inloop;
    ParameterHolder *ph;
    my_data = (nLoopColliParaN *) loopPara;

    bi = my_data->bi;
    ei = my_data->ei;
    inloop = my_data->inloop;
    ph = my_data->ph;

    for(int i = bi;i<ei;i++)
    {
        // inner loop
        {
            int q = i/inloop; // retrive q index
            int n = i%inloop; // retrive n index
            for (int hash_id = 0; hash_id < ph->L; ++hash_id) {
                if ((*my_data->hMatrixN)[n][hash_id] == (*my_data->hMatrixQ)[q][hash_id])
                    (*my_data->collisionMatrix)[q][n]++;
            }
        }
    }
    return NULL;
}

/**
 * the thread function to compute candidate set in normal mode
 * @param loopPara struct nLoopCandidateParaN
 * @return
 */
void* computeCandidateNormalPthreadFucN(void *loopPara)
{
    //retrieve the parameters
    struct nLoopCandidateParaN *my_data;
    int bi,ei;
    ParameterHolder *ph;

    my_data = (nLoopCandidateParaN *) loopPara;

    bi = my_data->bi;
    ei = my_data->ei;
    ph = my_data->ph;

    for(int i = bi;i<ei;i++)
    {
        // inner loop
        {
            vector1D candidates;
            for (int j = 0; j < ph->N; ++j) {
                if((*my_data->collisionMatrix)[i][j]>=ph->T)
                    candidates.push_back((double &&) j);
            }
            (*my_data->candidateSet)[i] = candidates;

        }
    }
    return NULL;
}

/**
 * the thread function to compute candidate set in quick mode
 * @param loopPara struct nLoopCandidateParaN
 * @return
 */
void* computeCandidatesQuickPthreadFucN(void *loopPara)
{
    struct nLoopCandidateParaN *my_data;
    int bi,ei;
    ParameterHolder *ph;
    my_data = (nLoopCandidateParaN *) loopPara;

    bi = my_data->bi;
    ei = my_data->ei;
    ph = my_data->ph;


    for(int i = bi;i<ei;i++)
    {
        // inner loop
        {
            vector1D singleRow(0, 0);
            //loop through N
            for (int n = 0; n <ph->N ; ++n){
                int colliNum = 0;
                // loop through L
                for (int hash_id = 0; hash_id < ph->L; ++hash_id) {
                    if ((*my_data->hMatrixN)[n][hash_id] == (*my_data->hMatrixQ)[i][hash_id]) {
                        colliNum++;
                        if(colliNum>=ph->T){
                            singleRow.push_back((double &&) n);
                            break;
                        }

                    }
                }
            }
            (*my_data->candidateSet)[i] = singleRow;
        }
    }
    return NULL;
}


vector2D ComputerPthread::computeHash(vector2D dataset, size_t pointNum){
    // pre-allocate memory
    vector2D hashMatrix;
    for (int i = 0; i < pointNum; ++i) {
        vector1D vL(ph.L,0);
        hashMatrix.push_back(vL);
    }

    // it is a implementation of nested loop
    const size_t nthreads = getNumOfCoresN();
    const size_t outloop = pointNum;
    const size_t inloop = ph.L;
    const size_t nloop = outloop*inloop;

    //prepare vector of threads and arguments for each thread
    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopHashParaFN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        // prepare parameters for each threads
        nLoopHashParaFN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.inloop = inloop;
        tempPara.dataset = &dataset;
        tempPara.hashMatrix = &hashMatrix;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

        // create thread and start work
        pthread_create(&threads[t], NULL, computeHashPthreadFucN, (void*)(&(thread_args[t])));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return hashMatrix;
}

vector2D ComputerPthreadNormal::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
    // prepare memory
    vector2D collisionMatrix;
    for (int i = 0; i < ph.Q ; ++i) {
        vector1D singleLine(ph.N, 0);
        collisionMatrix.push_back(singleLine);
    }

    // it is a implementation of nested loop
    const size_t nthreads = getNumOfCoresN();
    const size_t outloop = ph.Q;
    const size_t inloop = ph.N;
    const size_t nloop = outloop*inloop;

    //prepare vector of threads and arguments for each thread
    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopColliParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        // prepare parameters for each threads
        nLoopColliParaN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.inloop = inloop;
        tempPara.hMatrixN = &hMatrixN;
        tempPara.hMatrixQ = &hMatrixQ;
        tempPara.collisionMatrix = &collisionMatrix;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

        // create thread and start work
        pthread_create(&threads[t], NULL, computeCollisionPthreadFucN, (void*)&(thread_args[t]));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return collisionMatrix;
}


vector2D ComputerPthreadNormal::computeCandidate(vector2D collisionMatrix){
    // prepare memory
    vector2D candidateSet;
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

    // it is a implementation of nested loop
    const size_t nthreads = getNumOfCoresN();
    const size_t nloop = ph.Q;

    //prepare vector of threads and arguments for each thread
    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopCandidateParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        // prepare parameters for each threads
        nLoopCandidateParaN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.collisionMatrix = &collisionMatrix;
        tempPara.candidateSet = &candidateSet;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

        // create thread and start work
        pthread_create(&threads[t], NULL, computeCandidateNormalPthreadFucN, (void*)&(thread_args[t]));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return candidateSet;
}


vector2D ComputerPthreadQuick::computeCandidate(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D candidateSet;
    //Prepare set to store candidates
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

    const size_t nthreads = getNumOfCoresN();
    const size_t nloop = ph.Q;

    //prepare vector of threads and arguments for each thread
    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopCandidateParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        // prepare parameters for each threads
        nLoopCandidateParaN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.ph = &ph;
        tempPara.hMatrixN = &hMatrixN;
        tempPara.hMatrixQ = &hMatrixQ;
        tempPara.candidateSet = &candidateSet;
        thread_args[t] = tempPara;

        pthread_create(&threads[t], NULL, computeCandidatesQuickPthreadFucN, (void*)&(thread_args[t]));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return candidateSet;
}


void ComputerPthread::printThreadMode(){
    std::cout<< "Pthread";
}
void ComputerPthreadNormal::printComputeMode(){
    std::cout<< "Normal";
}
void ComputerPthreadQuick::printComputeMode(){
    std::cout<< "Quick";
}

