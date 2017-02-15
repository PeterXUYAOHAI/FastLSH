//
// Created by peter on 17-2-15.
//
#include <libmemcached/memcached.h>
#include <iostream>
#include <sstream>
#include "../include/LSH.h"


//remember to start the server:
// >>memcached -p 11211 -m 64m -d

//check if server is on
// telnet 127.0.0.1 11211


//server--server address, port--port number, exp--exprition time(how long it gonna stay in memory (0 for forever)
int LSH::saveHashNToMemc(const char* server, in_port_t port, time_t exp){

    //check if hashMatrixAlreadyExist
    if (hashMatrixN.size()<=0){
        fprintf(stderr, "hashMatrixN not exist\n Maybe you haven't calculate it yet");
        return 0;
    }


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

    fprintf(stderr, "Start to pip file into memcache");


    for (int i = 0; i < N; ++i) {
        std::string keyString = (runID+"HaN"+std::to_string(i));
        std::string valueString = "";
        for (int j = 0; j < L; ++j) {
            if (j<(L-1))
                valueString += (std::to_string(hashMatrixN[i][j])+",");
            else
                valueString += (std::to_string(hashMatrixN[i][j]));
        }
        rc = memcached_set(memc, keyString.c_str(), keyString.size(), valueString.c_str(), valueString.size(), (time_t)exp, (uint32_t)0);

        if (rc != MEMCACHED_SUCCESS) {
            fprintf(stderr, "Couldn't store key: %s, storage failed\n", memcached_strerror(memc, rc));
            return 0;
        }
    }

    fprintf(stderr, "HashMatrixN successfully stored, get them from memcache by %sHaN+queryIndex", runID);


}


//srunId--the specific runId of the hashmatrix
vector2D LSH::readHashNFromMemc(const char* server, in_port_t port,std::string srunId){

    if (hashMatrixN.size()>0){
        fprintf(stderr, "Are you sure to overwrite exist HashMatrixN?(Y/N)\n");
        char answ;
        std::cin>>answ;
        if(answ =='N')
            return NULL;
    }

    memcached_server_st *servers = NULL;
    memcached_st *memc;
    memcached_return rc;

    memc = memcached_create(NULL);
    servers = memcached_server_list_append(servers, server, port, &rc);
    rc = memcached_server_push(memc, servers);

    vector2D data;

    for (int i = 0; i < N; i++) {
        std::vector<double> temp(L, 0);
        std::string keyString = (srunId+"HaN"+std::to_string(i));
        char *retrieved_value;
        retrieved_value = memcached_get(memc, keyString, keyString.size(), &value_length, &flags, &rc);

        data.push_back(temp);
    }

    std::stringstream ss;

    ss<< buffer;

    //close hdfs related object, free memory
    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);


    //read csv file syntax

    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(ss, value, ',');
            else
                getline(ss,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    return data;

}





