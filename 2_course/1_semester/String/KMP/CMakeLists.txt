cmake_minimum_required(VERSION 2.8)

project(KMP) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(KMP KMP.h main.cpp)
