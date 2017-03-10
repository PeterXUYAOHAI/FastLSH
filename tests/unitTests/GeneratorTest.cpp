//
// Created by peter on 17-2-22.
//

#include <gtest/gtest.h>
#include <chrono>
#include "../../include/LSH.h"


//marco to clean the code
#define now() std::chrono::high_resolution_clock::now()
#define dcast std::chrono::duration_cast<std::chrono::microseconds>



//
// Created by peter on 17-2-7.
//

#include <gtest/gtest.h>
#include "../../include/LSH.h"
#include "../../include/otherClasses/Computer.h"


class GeneratorTest:public ::testing::Test{

protected:
    virtual void SetUp() {
        //preload data
        mlsh = LSH(1000, 57, 200, 1, 1.2, 1000, 100);
//            mlsh = LSH(300000, 57, 200, 1, 1.2, 1000);
        mlsh.loadSetN("../tests/dataset/dataset1000NoIndex.csv", 0);
        mlsh.loadSetQ("../tests/dataset/dataset1000NoIndex.csv", 0);

    }
    LSH mlsh;
    Generator *theGenrator;

};

TEST_F(GeneratorTest, geTest){

    theGenrator = new Generator(mlsh.ph);


    vector2D resultOrigin = mlsh.getCollisionMatrix();
    vector2D resultNormal = theGenrator->generateCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);

    vector2D hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    vector2D hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);

    vector2D candidateOrigin = mlsh.getCandidateSet();
    vector2D candidateNormal = theGenrator->generateCandidate(resultOrigin);

    vector2D hashQ = mlsh.hashMatrixQ;
    vector2D hashN = mlsh.hashMatrixN;

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);


     theGenrator->changeComputer(1,0);
     hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
     hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
     resultNormal = theGenrator->generateCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);
     candidateNormal = theGenrator->generateCandidate(resultOrigin);
//
    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);

    theGenrator->changeComputer(2,0);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    resultNormal = theGenrator->generateCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);
    candidateNormal = theGenrator->generateCandidate(resultOrigin);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
//
//
    theGenrator->changeComputer(3,0);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    resultNormal = theGenrator->generateCollision(mlsh.hashMatrixQ,mlsh.hashMatrixN);
    candidateNormal = theGenrator->generateCandidate(resultOrigin);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(resultOrigin, resultNormal);
    ASSERT_EQ(candidateOrigin, candidateNormal);
//
    theGenrator->changeComputer(0,1);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    candidateNormal = theGenrator->generateCandidate(mlsh.hashMatrixN,mlsh.hashMatrixQ);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(candidateOrigin, candidateNormal);

////
    theGenrator->changeComputer(1,1);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    candidateNormal = theGenrator->generateCandidate(mlsh.hashMatrixN,mlsh.hashMatrixQ);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(candidateOrigin, candidateNormal);
//
////
    theGenrator->changeComputer(2,1);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    candidateNormal = theGenrator->generateCandidate(mlsh.hashMatrixN,mlsh.hashMatrixQ);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(candidateOrigin, candidateNormal);
//
    theGenrator->changeComputer(3,1);
    hashQGenerator = theGenrator->generateHash(mlsh.setQ,mlsh.ph.Q);
    hashNGenerator = theGenrator->generateHash(mlsh.setN,mlsh.ph.N);
    candidateNormal = theGenrator->generateCandidate(mlsh.hashMatrixN,mlsh.hashMatrixQ);

    //compare if two hash result are same
    ASSERT_EQ(hashQ,hashQGenerator);
    ASSERT_EQ(hashN, hashNGenerator);
    ASSERT_EQ(candidateOrigin, candidateNormal);


}





