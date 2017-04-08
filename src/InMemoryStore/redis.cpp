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
    redis.cpp
    Purpose: This is the source file for redis functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/



#include <libmemcached/memcached.h>
#include <iostream>
#include <sstream>
#include "../../include/LSH.h"
#include <hiredis/hiredis.h>

//server--server address, port--port number
int LSH::saveHashNToRedis(const char* server, in_port_t port){

    //check if hashMatrixAlreadyExist
    if (hashMatrixN.size()<=0){
        fprintf(stderr, "hashMatrixN not exist, Maybe you haven't calculate it yet\n");
        return 1;
    }

    //connect the redis server
    redisReply *reply;
    redisContext *c = redisConnect(server, port);

    if ( c->err)
    {
        redisFree(c);
        printf("Connect to redisServer failed\n");
        return 0;
    }

    printf("Connect to redisServer Success\n");

    fprintf(stderr, "Start to pip file into redis\n");

    // construct the string for storage
    for (int i = 0; i < ph.N; ++i) {
        std::string keyString = (runID+"HaN"+std::to_string(i));
        std::string valueString = "";
        std::string cmd = "SET ";
        for (int j = 0; j < ph.L; ++j) {
            if (j<(ph.L-1))
                valueString += (std::to_string(hashMatrixN[i][j])+","); // the value won't be exactly the same, because it is double
            else
                valueString += (std::to_string(hashMatrixN[i][j])+"\n");
        }
        cmd+=keyString;
        cmd+=" ";
        cmd+=valueString;

        reply = (redisReply*)redisCommand( c, cmd.c_str());

        if (strcmp(reply->str, "OK") != 0 ) {
            fprintf(stderr, "Couldn't store key: %s, storage failed, %s\n", keyString.c_str(), reply->str);
            return 0;
        }
    }

    fprintf(stderr, "HashMatrixN successfully stored, get them from memcache by %sHaN+queryIndex\n", runID.c_str());

    redisFree(c);

    return 0;

}


//srunId--the specific runId of the hashmatrix
int LSH::readHashNFromRedis(const char* server, in_port_t port, std::string srunId){

/* disabled for the gtest
    if (hashMatrixN.size()>0){
        fprintf(stderr, "Are you sure to overwrite exist HashMatrixN?(Y/N)\n");
        char answ;
        std::cin>>answ;
        if(answ =='N')
            return ;
    }
*/
    //connect the redis server
    redisReply *reply;
    redisContext *c = redisConnect(server, port);

    if ( c->err)
    {
        redisFree(c);
        printf("Connect to redisServer failed\n");
        return 1;
    }

    printf("Connect to redisServer Success\n");

    std::string value;
    std::string num;


    vector2D data;

    //read from redis server, parse and reconstruct it into hashN vector
    for (int i = 0; i < ph.N; i++) {
        std::vector<double> temp(ph.L, 0);
        std::string keyStringCmd = "GET "+(srunId+"HaN"+std::to_string(i));
        char *retrieved_value;
        std::stringstream ss;

        reply = (redisReply*)redisCommand( c, keyStringCmd.c_str());

        retrieved_value = reply->str;
        ss<< retrieved_value;
        free(retrieved_value);

        for (int j = 0; j<ph.L; j++) {
            if (j != ph.L - 1)
                getline(ss, value, ',');
            else
                getline(ss, value, '\n');
            num = std::string(value, 0, value.length());
            temp[j] = ::atof(num.c_str());
        }
        data.push_back(temp);
    }
    hashMatrixN = data;

    redisFree(c);

    return 0;
}








