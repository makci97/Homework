cmake_minimum_required(VERSION 2.8)

project(tree_mutex) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(tree_mutex tree_mutex.h main.cpp)
