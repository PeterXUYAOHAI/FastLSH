//
// Created by peter on 17-2-17.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "hdfs.h"
#include <gtest/gtest.h>
#include <thread>
#include <mutex>

#include <mpi.h>


typedef std::vector<std::vector<std::vector<double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;



vector2D loadDataFromLinuxSystem(char* filePath, size_t row, size_t col) {
    std::ifstream file;// declare file stream:
    file.open(filePath);
    vector2D data;
    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }
    std::string value;
    std::string num;
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(file, value, ',');
            else
                getline(file,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    file.close();
    return data;
}



int main (int argc, char **argv) {

    //distable for now
    exit(0);

    int my_id, root_process, ierr, num_procs;
    MPI_Status status;

    ierr = MPI_Init(&argc, &argv);

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    root_process = 0;

    size_t N = 1000; //# of vectors in the dataset
    size_t Q = 1000; //# of vertors in the queryset
    size_t D = 57; //# of dimensions
    size_t L = 200; //# of group hash
    size_t K = 1; //# the number of hash functions in each group hash
    double W = 1.2; //bucket width



    if(my_id == root_process) {

        vector3D randomLine;
        vector1D UniformRandomVector;


            //generate random line

                std::random_device rd;
                std::mt19937 gen(rd());

                for (int i = 0; i < L; i++) {
                    vector2D vK;
                    for (int j = 0; j < K; ++j) {
                        //generate random according normal distribution
                        std::normal_distribution<double> distribution(0.5, 0.5);
                        std::vector<double> vD;
                        for (int k = 0; k < D; ++k) {
                            vD.push_back(distribution(gen));
                        }
                        vK.push_back(vD);
                    }
                    randomLine.push_back(vK);
                }



            std::uniform_real_distribution<> dis(0, 1.2);
            for (int i = 0; i < 1; ++i) {
                UniformRandomVector.push_back(dis(gen));
            }

        vector2D setN; // original data set of N
        vector2D setQ; // original data set of Q
        vector2D hashMatrixN; // hashMatrix of N where hash value is stored
        vector2D hashMatrixQ; // hashMatrix of Q where hash value is stored


        setN = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", N, D);
        setQ = loadDataFromLinuxSystem("../tests/dataset/dataset1000NoIndex.csv", Q, D);


        //this part is for send set Q
        {



        }
        //this part is for send random values
        {


        }
        //this part is for send part of set N
        {


        }
        //this part is for send other variables
        {


        }
        //this part is for receive part of the collision table
        {


        }
        //this part is for merge the collision table
        {


        }






//        openMPResult = mlsh.getCollisionMatrix();
//        hashMatrixN = computeHash(setN, N);
//        hashMatrixQ = computeHash(setQ, Q);
//        collisionMatrix = computeCollision(hashMatrixN, hashMatrixQ);

    }
    else {



        /* I must be slave process, so I must receive my array segment,

         * calculate the sum of my portion of the array,

         * and, finally, send my portion of the sum to the root process. */

    }

    /* Stop this process */









    std::cout<<"get it";
//
//    ierr = MPI_Finalize();

        // Initialize the MPI environment
//    MPI_Init(NULL, NULL);
//
//    // Get the number of processes
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    // Get the rank of the process
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//
//    // Get the name of the processor
//    char processor_name[MPI_MAX_PROCESSOR_NAME];
//    int name_len;
//    MPI_Get_processor_name(processor_name, &name_len);
//
//    std::cin>>name_len;
//    // Print off a hello world message
//    printf("Hello world from processor %s, rank %d"
//                   " out of %d processors\n",
//           processor_name, world_rank, world_size);
//
//    // Finalize the MPI environment.
//    MPI_Finalize();
//
//    std::cout<<"yes";
//
//    std::cin>>name_len;
}
