//
// Created by peter on 17-3-9.
//

#ifndef FASTLSH_COMPUTER_H
#define FASTLSH_COMPUTER_H

class Computer{

public:
    Computer(size_t &T,size_t &K,size_t &D, double &W, size_t &Q, size_t &N, size_t &T, vector3D &randomLine, vector1D &randomVector
    ): L(L),K(K),D(D),W(W),Q(Q), N(N), T(T), randomLine(randomLine),randomVector(randomVector){};
    virtual vector2D computeHash()=0;
    virtual vector2D computeCandidate()=0;

protected:
    size_t L;
    size_t K;
    size_t D;
    size_t Q;
    size_t N;
    size_t T;
    double W;
    vector3D randomLine;
    vector1D randomVector;
};


class ComputerSingleThread: public Computer{

public:
    vector2D computeHash(vector2D dataset, size_t pNum);
    virtual vector2D computeCandidate()=0;
};


class ComputerSingleThreadNormal: public  ComputerSingleThread{

public:
    vector2D computeCollision(vector2D hMatrixN, vector2D hMatrixQ);
    vector2D computeCandidate(vector2D collisionMatrix);
};


class ComputerSingleThreadQuick: public  ComputerSingleThread{
public:
    virtual vector2D computeCandidate(vector2D hMatrixN, vector2D hMatrixQ, size_t T);
};



//class ComputerOpenMP: public Computer{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//
//class ComputerStdThread: public  Computer{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//class ComputerPthread: public  Computer{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//
//
//class ComputerOpenMPNormal: public ComputerOpenMP{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//class ComputerOpenMPQuick: public ComputerOpenMP{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//
//class ComputerStdThreadNormal: public  ComputerStdThread{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//class ComputerStdThreadQuick: public  ComputerStdThread{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//class ComputerPthreadNormal: public  ComputerPthread{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};
//
//class ComputerPthreadQuick: public  ComputerPthread{
//
//public:
//    vector2D computeHash();
//    virtual vector2D computeCandidate()=0;
//};

#endif //FASTLSH_COMPUTER_H
