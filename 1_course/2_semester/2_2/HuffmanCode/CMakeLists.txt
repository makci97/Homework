cmake_minimum_required(VERSION 2.8)

project(HuffmanCode) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(HuffmanCode HuffmanCode.cpp main.cpp)
