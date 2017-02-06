# FastLSH

[![Build Status](https://travis-ci.org/PeterXUYAOHAI/FastLSH.svg?branch=master)](https://travis-ci.org/PeterXUYAOHAI/FastLSH)

This is a C++ implementation of E2LSH and C2LSH. More detail to come.
The build won't pass now because  travis don't support hadoop environment. I will try to fix it.


##Parameters
* ```N``` the number of vectors in the N dataset
* ```Q``` the number of vectors in the query dataset
* ```D``` the number of dimensions
* ```L``` the number of group hash
* ```K``` the number of lines in each hash group -- the larger the value, the fewer the collisions
* ```W``` the number of bucket width

## Authors

* **Peter XU **  [PeterXUYAOHAI](https://github.com/PeterXUYAOHAI)


## Acknowledgments

* Gan, J., Feng, J., Fang, Q., & Ng, W. (2012, May). Locality-sensitive hashing scheme based on dynamic collision counting. In Proceedings of the 2012 ACM SIGMOD International Conference on Management of Data (pp. 541-552). ACM.



