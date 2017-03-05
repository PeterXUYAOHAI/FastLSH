# FastLSH

[![Build Status](https://travis-ci.org/PeterXUYAOHAI/FastLSH.svg?branch=master)](https://travis-ci.org/PeterXUYAOHAI/FastLSH)

This is a C++ implementation of E2LSH and C2LSH. More detail to come.
The build won't pass now because  travis don't support hadoop environment. I will try to fix it.

##Attention:
The current version requires certain programs installedfor the full functionality.
It has been tested on Ubuntu 16.04 with the following programs
* Hadoop 2.7.3
* OpenMP 4.5
* Memcached 1.4.34
* Redis 3.2
* OpenMPI 2.02
* Boost 1.5
* Spark 2.1.0

If you can not fulfill the requisites, try to use CMakeList_safe.txt in the repo and comment the lines where compile error happens.(i.e. Line 32-34 and 42-43 in src/LSH.cpp as well as 9 and 14 in test/driver.cpp) I will creat a safa brach soon.


##Parameters
* ```N``` the number of vectors in the N dataset
* ```Q``` the number of vectors in the query dataset
* ```D``` the number of dimensions
* ```L``` the number of group hash
* ```K``` the number of lines in each hash group -- the larger the value, the fewer the collisions
* ```W``` the number of bucket width

## TODO
* ~~Access to MPI~~
* ~~Access to Redis~~
* ~~Access to memcached~~
* Distributed computing
* ~~Parameter generator~~
* Rename function names
* Rafactor, apply design patterns

## System Structure 
![system structure](https://cloud.githubusercontent.com/assets/11495951/23577931/713716c4-0106-11e7-9cc2-a0338c80d3b2.PNG)

## Authors

* *Peter XU*  [PeterXUYAOHAI](https://github.com/PeterXUYAOHAI)


## Acknowledgments

* Gan, J., Feng, J., Fang, Q., & Ng, W. (2012, May). Locality-sensitive hashing scheme based on dynamic collision counting. In Proceedings of the 2012 ACM SIGMOD International Conference on Management of Data (pp. 541-552). ACM.



