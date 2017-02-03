cmake_minimum_required(VERSION 3.6)
project(FastLSH)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        README.md tests/main.cpp include/LSH.h)

add_executable(FastLSH ${SOURCE_FILES})