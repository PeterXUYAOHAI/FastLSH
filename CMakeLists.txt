cmake_minimum_required(VERSION 3.2)
project(FastLSH)


# Include the GoogleTest directory
set(GTEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest/googletest)
# Add GoogleTest as a build subdirectory
add_subdirectory(lib/googletest)
# Include the GoogleTest header directory
include_directories(${GTEST_DIRECTORY}/include)

#import OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


#import libhdfs
set(HDFS_FOUND "NO")
find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()


if(JAVA_INCLUDE_PATH)

    find_path( HDFS_INCLUDE_PATH NAMES hdfs.h PATHS /usr/local/hadoop/include )
    find_path( HDFS_LIBRARY_PATH libhdfs.so PATHS /usr/local/hadoop/lib/native )
    find_library( HDFS_LIBRARY NAMES libhdfs.so PATHS /usr/local/hadoop/lib/native  )
    if(HDFS_INCLUDE_PATH)
        if(HDFS_LIBRARY_PATH)
            if(HDFS_LIBRARY)
                set( HDFS_FOUND "YES")
                set( HDFS_INCLUDE_DIRS ${HDFS_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} )
                set( HDFS_LIBRARIES ${HDFS_LIBRARY} ${JNI_LIBRARIES} )
                message( STATUS "Found HDFS" ${HDFS_INCLUDE_PATH} )

            endif()
        endif()
    endif()
endif()

#MPI
#complie with openmp
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

#find_package(MPI)
#include_directories(SYSTEM ${MPI_INCLUDE_PATH})


find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)

#message(${HDFS_LIBRARIES})
#
#message($ENV{CLASSPATH})
#
#message($ENV{test})

include_directories( ${HDFS_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        README.md tests/driver.cpp include/LSH.h src/LSH.cpp
        tests/unitTests/HdfsTest.cpp tests/unitTests/MetaTest.cpp tests/unitTests/MTTests/StdthreadTest.cpp
        tests/unitTests/MTTests/PthreadTest.cpp src/InMemoryStore/memcached.cpp
        tests/unitTests/MTTests/OpenMPTest.cpp
        tests/unitTests/inMemoryStoreTest/MemcachedTest.cpp
        src/InMemoryStore/redis.cpp tests/unitTests/inMemoryStoreTest/RedisTest.cpp
        src/Normalizer.cpp tests/unitTests/SingleThreadTest.cpp
        include/otherClasses/FileLoader.h
        include/otherClasses/Computer.h tests/unitTests/ComputerTest.cpp include/otherClasses/ParameterHolder.h
        include/otherClasses/Generator.h src/Generator.cpp src/Computer/PthreadComputer.cpp tests/unitTests/GeneratorTest.cpp src/Computer/SingleThreadComputer.cpp src/Computer/OpenMPComputer.cpp src/Computer/StdThreadComputer.cpp src/FileLoader/LfsLoader.cpp src/FileLoader/FileLoader.cpp src/FileLoader/HdfsLoader.cpp tests/helper.cpp)

set(MPI_FILES src/ClusterCompute/mpiCompute.cpp)
set(BoostMPI_FILES src/ClusterCompute/boostmpiCompute.cpp include/otherClasses/Computer.h)
set(Paragen_FILES src/paragen.cpp)

#find boost library
find_package (Boost 1.55.0 REQUIRED COMPONENTS system filesystem mpi)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)


message(${MPIEXEC})

message(${SOURCE_FILES})

message(${MPI_LIBRARIES})

add_executable(mpiLSH ${MPI_FILES})
add_executable(FastLSH ${SOURCE_FILES})
add_executable(boostmpiLSH ${BoostMPI_FILES})
add_executable(paraGen ${Paragen_FILES})
#add_library(FastLSH ${SOURCE_FILES})


target_link_libraries (FastLSH ${HDFS_LIBRARIES} gtest memcached hiredis mpi)
#target_link_libraries(mpiLSH ${MPI_LIBRARIES})
target_link_libraries(boostmpiLSH ${Boost_LIBRARIES} ${MPI_LIBRARIES})



#add_custom_command(TARGET mpiLSH
#        COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4
#        ${MPIEXEC_PREFLAGS} mpiLSH
#        )

add_custom_command(TARGET boostmpiLSH
        COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4
        ${MPIEXEC_PREFLAGS} boostmpiLSH
        )