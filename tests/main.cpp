#include <iostream>
#include <vector>

#include "../include/LSH.h"


int main (int argc, char **argv){
    std::cout<<"hello world!";

    LSH mlsh(1000, 57, 200, 1, 1.2, 1000);

    //here we use same set for N and query set
    mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv");
    mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv");

    vector2D collisionMatrix;
    collisionMatrix = mlsh.getCollisionMatrix();

//    std::cout<<collisionMatrix.size();
//    std::cout<<collisionMatrix[0].size();

//  gengerate random test
//    LSH mlsh;

//    vector3D temp = mlsh.generateRandomLine(200,3,50);
//    std::cout<<temp.size();
//    std::cout<<temp[0].size();
//    std::cout<<temp[0][0].size();
//    std::cout<<temp[0][0][0];
//    std::vector<double> v = mlsh.generateUniformRandomVector(10,1.3);
//    std::cout<<v.size();
//    std::cout<<v[0];

// load file test
//    {
//    LSH mlsh;
//    std::vector<std::vector<double>>  test;
//    test = mlsh.loadData("../tests/dataset/dataset1000NoIndex.csv", 1000, 57);
//    std::cout<<test[0][2];
//    }

//compute hashmatrix
//    vector2D hashmatrix;
//    hashmatrix = mlsh.computeHash(test,200,1.2,1000,57,3);
//    std::cout<<hashmatrix[0][0]<<std::endl;
//    std::cout<<hashmatrix.size()<<std::endl;
//    std::cout<<hashmatrix[0].size()<<std::endl;



}