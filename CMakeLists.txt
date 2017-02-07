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

#
#message(${HDFS_LIBRARIES})
#
#message($ENV{CLASSPATH})
#
#message($ENV{test})

include_directories( ${HDFS_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        README.md tests/driver.cpp include/LSH.h src/LSH.cpp src/hdfsRead.cpp src/computeMt-openMP.cpp src/computeMt-stdthread.cpp src/computeMt-pthread.cpp src/compute.cpp tests/mt-OpenMP_test.cpp)

add_executable(FastLSH ${SOURCE_FILES})
target_link_libraries (FastLSH ${HDFS_LIBRARIES} gtest)
