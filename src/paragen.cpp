//
// Created by peter on 17-2-20.
//


#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <sstream>

double psFunction(double t, double S, double W){

    //separate the function for clear representation
    double part1 = (1/S);
    double part2_1 = (2/(sqrt(2*M_PI)));
    double part2_2 = exp(-(pow(t/S,2))/2);
    double part3 = (1-t/W);
    double result = part1*part2_1*part2_2*part3;

    return result;
}



double pIntegralGenerator(int numOfSlice, double W, double S){
    double lenOfSlice;
    lenOfSlice = W/numOfSlice;

    double integral = 0;
    for (int i = 0; i < numOfSlice; ++i) {
        integral += psFunction(lenOfSlice*(i+i+1)/2,S,W)*lenOfSlice;
    }

    return integral;

}

double getZ(double Theta,double Delta){
    double Z = sqrt(log(2/Theta)/log(1/Delta));
    return Z;
}


double getAlpha(double Z, double P1, double P2){
    double Alpha = (Z*P1+P2)/(1+Z);
    return Alpha;
}

double getL(double Delta, double P1, double P2, double Z){

    double part1_up = log(1/Delta);
    double part1_down = 2*pow((P1-P2),2);
    double part2 = pow((1+Z),2);
    double L = part1_up/part1_down*part2;

    return L;
}


int main (int argc, char **argv) {



    double W; //bucket width
    double R; //query range
    double C; //approximation ratio
    double Alpha; //collision percentage
    double Delta; //false negative rate
    double Theta; //false positive rate
    double L; //the number of hash functions
    double K; //#projections in each hash function
    double T; //collision threshold (T = Alpha*L)
    double D; //number of dimensions

    int numOfSlice = 1000; //num of slice for integral calculation, the higher the slower but more percise
    double P1;
    double P2;
    double Z;



    std::cout<<".______      ___      .______          ___      .___  ___.  _______ .___________. _______ .______      \n"
            "|   _  \\    /   \\     |   _  \\        /   \\     |   \\/   | |   ____||           ||   ____||   _  \\     \n"
            "|  |_)  |  /  ^  \\    |  |_)  |      /  ^  \\    |  \\  /  | |  |__   `---|  |----`|  |__   |  |_)  |    \n"
            "|   ___/  /  /_\\  \\   |      /      /  /_\\  \\   |  |\\/|  | |   __|      |  |     |   __|  |      /     \n"
            "|  |     /  _____  \\  |  |\\  \\----./  _____  \\  |  |  |  | |  |____     |  |     |  |____ |  |\\  \\----.\n"
            "| _|    /__/     \\__\\ | _| `._____/__/     \\__\\ |__|  |__| |_______|    |__|     |_______|| _| `._____|\n"
            "                                                                                                       \n"
            "  _______  _______ .__   __.  _______ .______          ___   .___________.  ______   .______           \n"
            " /  _____||   ____||  \\ |  | |   ____||   _  \\        /   \\  |           | /  __  \\  |   _  \\          \n"
            "|  |  __  |  |__   |   \\|  | |  |__   |  |_)  |      /  ^  \\ `---|  |----`|  |  |  | |  |_)  |         \n"
            "|  | |_ | |   __|  |  . `  | |   __|  |      /      /  /_\\  \\    |  |     |  |  |  | |      /          \n"
            "|  |__| | |  |____ |  |\\   | |  |____ |  |\\  \\----./  _____  \\   |  |     |  `--'  | |  |\\  \\----.     \n"
            " \\______| |_______||__| \\__| |_______|| _| `._____/__/     \\__\\  |__|      \\______/  | _| `._____|\n";

    std::cout<< "Please input the following parameters \n";

    std::cout<< "bucket width(W) [default=1]: ";
    std::string input;
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> W;
    }
    else{
        W = 1;
    }

    std::cout<< "approximation ratio(C) [default=1.5]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> C;
    }
    else{
        C = 1.5;
    }

    std::cout<< "false negative rate(Delta) [default=0.05]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> Delta;
    }
    else{
        Delta = 0.05;
    }

    std::cout<< "false positive rate(Theta) [default=0.3]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> Theta;
    }
    else{
        Theta = 0.3;
    }

    std::cout<< "projections in each hash(K) function [default=1]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> K;
    }
    else{
        K = 1;
    }

    std::cout<< "the number of dimensions in your dataset [default=56]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        std::istringstream stream(input);
        stream >> D;
    }
    else{
        D = 56;
    }

    std::cout<< "the Range parameter(0-1, the higher the higher the bigger the value) [default=0.12]: ";
    std::getline(std::cin, input);
    if(!input.empty()){
        double ran_para;
        std::istringstream stream(input);
        stream >> ran_para;
        R = ran_para * sqrt(D);
    }
    else{
        R = 0.067 * sqrt(D);
    }

    // parameter generation
    P1 = pIntegralGenerator(numOfSlice, W, R);
    P2 = pIntegralGenerator(numOfSlice, W, C*R);
    Z = getZ(Theta, Delta);
    Alpha = getAlpha(Z, P1, P2);
    L =getL(Delta,P1, P2, Z);
    T = Alpha*L;


    std::cout<<"Here is your parameters: \n";
    std::cout<<"L (the number of hash funcitons) = "<<L<<std::endl;
    std::cout<<"T (collision threshold) = "<<T<<std::endl;
    std::cout<<"R (query range) = "<<R<<std::endl;
    std::cout<<"Alpha (collision percentage) = "<<Alpha<<std::endl;
    std::cout<<"W (bucket width) [user input] =  "<<W<<std::endl;
    std::cout<<"C (approximation ratio) [user input] =  "<<C<<std::endl;
    std::cout<<"Delta (false negative rate) [user input] =  "<<Delta<<std::endl;
    std::cout<<"Theta (false negative rate) [user input] =  "<<Theta<<std::endl;
    std::cout<<"K (# projections in each hash function) [user input] =  "<<K<<std::endl;

}