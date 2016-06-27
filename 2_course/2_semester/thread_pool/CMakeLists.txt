cmake_minimum_required(VERSION 2.8)

project(thread_pool)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DCMAKE_BUILD_TYPE=debug")

add_executable(thread_pool thread_pool.h main.cpp)
