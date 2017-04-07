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
    memcached.cpp
    Purpose: This is the source file for memcached functions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/



/**
 * remember to start the server:
 * >>memcached -p 11211 -m 64m -d
 *
 * check if server is on
 * telnet 127.0.0.1 11211
 */


#include <libmemcached/memcached.h>
#include <iostream>
#include <sstream>
#include "../../include/LSH.h"

//server--server address, port--port number, exp--exprition time(how long it gonna stay in memory (0 for forever)
int LSH::saveHashNToMemcached(const char *server, in_port_t port, time_t exp){

    //check if hashMatrixAlreadyExist
    if (hashMatrixN.size()<=0){
        fprintf(stderr, "hashMatrixN not exist, Maybe you haven't calculate it yet\n");
        return 1;
    }

    //connect to memcached
    memcached_server_st *servers = NULL;
    memcached_st *memc;
    memcached_return rc;

    memc = memcached_create(NULL);
    servers = memcached_server_list_append(servers, server, port, &rc);
    rc = memcached_server_push(memc, servers);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Add server successfully\n");
    else
        fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));

    fprintf(stderr, "Start to pip file into memcache\n");


    // construct the string for storage
    for (int i = 0; i < ph.N; ++i) {
        std::string keyString = (runID+"HaN"+std::to_string(i));
        std::string valueString = "";
        for (int j = 0; j < ph.L; ++j) {
            if (j<(ph.L-1))
                valueString += (std::to_string(hashMatrixN[i][j])+","); // the value won't be exactly the same, because it is double
            else
                valueString += (std::to_string(hashMatrixN[i][j])+"\n");
        }
        rc = memcached_set(memc, keyString.c_str(), keyString.size(), valueString.c_str(), valueString.size(), (time_t)exp, (uint32_t)0);

        if (rc != MEMCACHED_SUCCESS) {
            fprintf(stderr, "Couldn't store key: %s, storage failed\n", memcached_strerror(memc, rc));
            return 0;
        }
    }

    fprintf(stderr, "HashMatrixN successfully stored, get them from memcache by %sHaN+queryIndex\n", runID.c_str());

    return 0;

}


//srunId--the specific runId of the hashmatrix
void LSH::readHashNFromMemcached(const char *server, in_port_t port, std::string srunId){

/*
     disabled for the gtest
    if (hashMatrixN.size()>0){
        fprintf(stderr, "Are you sure to overwrite exist HashMatrixN?(Y/N)\n");
        char answ;
        std::cin>>answ;
        if(answ =='N')
            return ;
    }
*/
    //connect the memcached server
    memcached_server_st *servers = NULL;
    memcached_st *memc;
    memcached_return rc;
    size_t value_length;
    uint32_t flags;

    memc = memcached_create(NULL);
    servers = memcached_server_list_append(servers, server, port, &rc);
    rc = memcached_server_push(memc, servers);
    std::string value;
    std::string num;


    vector2D data;

    //read from memcached server, parse and reconstruct it into hashN vector
    for (int i = 0; i < ph.N; i++) {
        std::vector<double> temp(ph.L, 0);
        std::string keyString = (srunId+"HaN"+std::to_string(i));
        char *retrieved_value;
        std::stringstream ss;

        retrieved_value = memcached_get(memc, keyString.c_str(), keyString.size(), &value_length, &flags, &rc);
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
}





