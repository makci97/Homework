cmake_minimum_required(VERSION 2.8)

project(Graph) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Graph Graph.h main.cpp)
