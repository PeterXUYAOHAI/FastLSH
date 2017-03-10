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
        README.md tests/driver.cpp include/LSH.h src/LSH.cpp src/compute/MTCompute/computeMt-openMP.cpp
        src/compute/MTCompute/computeMt-stdthread.cpp
        src/compute/MTCompute/computeMt-pthread.cpp
        src/compute/compute.cpp tests/unitTests/MTTests/mt-OpenMPTest.cpp
        tests/unitTests/hdfsTest.cpp tests/unitTests/metaTest.cpp tests/unitTests/MTTests/mt-stdthreadTest.cpp
        tests/unitTests/MTTests/mt-pthreadTest.cpp src/inMemoryStore/memcached.cpp
        tests/unitTests/inMemoryStoreTest/memcachedTest.cpp
        src/inMemoryStore/redis.cpp tests/unitTests/inMemoryStoreTest/redisTest.cpp
        src/normalization.cpp tests/unitTests/singleThreadTest.cpp
        src/FileLoader.cpp include/FileLoader.h
        include/Computer.cpp include/Computer.h tests/unitTests/computerTest.cpp include/ParameterHolder.h
        include/Generator.h include/Generator.cpp include/PthreadComputer.cpp)

set(MPI_FILES src/compute/clusterCompute/mpiCompute.cpp)
set(BoostMPI_FILES src/compute/clusterCompute/boostmpiCompute.cpp include/Computer.cpp include/Computer.h)
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