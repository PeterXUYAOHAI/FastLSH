//
// Created by peter on 17-3-10.
//
#include <pthread.h>
#include <random>
#include <iostream>
#include <algorithm>

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


struct nLoopHashParaFN{
    int bi;
    int ei;
    int inloop;
    ParameterHolder *ph;
    vector2D* hashMatrix;
    vector2D* dataset;
};

struct nLoopColliParaN{
    int bi;
    int ei;
    int inloop;
    ParameterHolder *ph;
    vector2D* collisionMatrix;
    vector2D* hMatrixQ;
    vector2D* hMatrixN;
};

struct nLoopCandidateParaN{
    int bi;
    int ei;
    ParameterHolder *ph;
    vector2D* hMatrixQ;
    vector2D* hMatrixN;
    vector2D *collisionMatrix;
    vector2D *candidateSet;

};


void* computeHashPthreadFucN(void *loopPara)
{
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
            // (optional) make output critical
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
            (*(my_data->hashMatrix))[n][l] = hashFinalValue;

        }
    }
    return NULL;
}



void* computeCollisionPthreadFucN(void *loopPara)
{
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
            int q = i/inloop;
            int n = i%inloop;
            for (int hash_id = 0; hash_id < ph->L; ++hash_id) {
                if ((*my_data->hMatrixN)[n][hash_id] == (*my_data->hMatrixQ)[q][hash_id])
                    (*my_data->collisionMatrix)[q][n]++;
            }
        }
    }
    return NULL;
}


void* computeCandidateNormalPthreadFucN(void *loopPara)
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
            for (int n = 0; n <ph->N ; ++n){
                int colliNum = 0;
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
    vector2D hashMatrix;

    for (int i = 0; i < pointNum; ++i) {
        vector1D vL(ph.L,0);
        hashMatrix.push_back(vL);
    }

    // it is a implementation of nested loop to stdthread
    const size_t nthreads = getNumOfCoresN();
    const size_t outloop = pointNum;
    const size_t inloop = ph.L;
    const size_t nloop = outloop*inloop;


    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopHashParaFN> thread_args(nthreads);
    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        nLoopHashParaFN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.inloop = inloop;
        tempPara.dataset = &dataset;
        tempPara.hashMatrix = &hashMatrix;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

        pthread_create(&threads[t], NULL, computeHashPthreadFucN, (void*)(&(thread_args[t])));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return hashMatrix;
}


vector2D ComputerPthreadNormal::computeCollision(vector2D hMatrixN, vector2D hMatrixQ){
    vector2D collisionMatrix;
    for (int i = 0; i < ph.Q ; ++i) {
        vector1D singleLine(ph.N, 0);
        collisionMatrix.push_back(singleLine);
    }

    const size_t nthreads = getNumOfCoresN();
    const size_t outloop = ph.Q;
    const size_t inloop = ph.N;
    const size_t nloop = outloop*inloop;

    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopColliParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        nLoopColliParaN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.inloop = inloop;
        tempPara.hMatrixN = &hMatrixN;
        tempPara.hMatrixQ = &hMatrixQ;
        tempPara.collisionMatrix = &collisionMatrix;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

        pthread_create(&threads[t], NULL, computeCollisionPthreadFucN, (void*)&(thread_args[t]));
    }

    /* wait for threads to finish */
    for (int t=0; t<nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    return collisionMatrix;
}


vector2D ComputerPthreadNormal::computeCandidate(vector2D collisionMatrix){
    vector2D candidateSet;
    for (int i = 0; i < ph.Q; ++i) {
        vector1D temp(0,0);
        candidateSet.push_back(temp);
    }

    const size_t nthreads = getNumOfCoresN();
    const size_t nloop = ph.Q;

    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopCandidateParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
        nLoopCandidateParaN tempPara;
        tempPara.bi = t*nloop/nthreads;
        tempPara.ei = (t+1)==nthreads?nloop:(t+1)*nloop/nthreads;
        tempPara.collisionMatrix = &collisionMatrix;
        tempPara.candidateSet = &candidateSet;
        tempPara.ph = &ph;
        thread_args[t] = tempPara;

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

    std::vector<pthread_t> threads(nthreads);
    std::vector<nLoopCandidateParaN> thread_args(nthreads);

    /* spawn the threads */
    for (int t=0; t<nthreads; ++t)
    {
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


