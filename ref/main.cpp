//
//  main.cpp
//  CollisionCounting
//
//  Created by LiHangyu on 16/6/1.
//  Copyright © 2016年 LiHangyu. All rights reserved.
//

#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 2
#define NQ 4000
#define NP 50000
#define D   200
#define Threshold   98
#define ThresholdQP  98
#define QS 10000

//load dataset from files

float**  load_data(const char* filename, int row, int col)
{
    ifstream file;// declare file stream:
    file.open(filename);
    string value;
    string num;
    float **data = new float*[row];
    for (int i = 0; i < row; i++)
        data[i] = new float[col];
    
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            getline(file, value, ',');
            num = string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
        getline(file, value, '\n');
    }
    file.close();
    return data;
}



// Computer hit matrix for datasetQ and datasetP
short **ComputeHitmatrix(float **hash_matrix1, float **hash_matrix2, int row1, int row2)
{
    int i, j, hash_id;
    short **hit_matrix = new short*[row1];
    for (int i = 0; i < row1; i++)
        hit_matrix[i] = new short[row2];
    for (i = 0; i < row1; i++)
        for (j = 0; j < row2; j++)
            hit_matrix[i][j] = 0;
    
    for (i = 0; i < row1; i++)
    {
        for (j = 0; j < row2; j++)
        {
            for (hash_id = 0; hash_id < D; hash_id++)
            {
                if (hash_matrix1[i][hash_id] == hash_matrix2[j][hash_id])
                    hit_matrix[i][j] += 1;
            }
        }
    }
    return hit_matrix;
}


int main(int argc, char* argv[])
{  //load datasetQ and datasetP
    float **hashQ, **hashP;
    short **hitQP;
    clock_t start, finish;
    double duration;
    start= clock();
    hashQ = load_data("hashQ.csv", NQ, D);
    hashP = load_data("hashP.csv", NP, D);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time for load datasets is :" << duration << endl;
    
    // Computer hit_matrix
    start = clock();
    hitQP = ComputeHitmatrix(hashQ, hashP, NQ, NP);
    cout << hitQP[0][0] <<endl;
    
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time to Computer hit matrix is :" << duration << endl;
    return 0;
}
