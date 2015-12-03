cmake_minimum_required(VERSION 2.8)

project(Suffix_tree) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Suffix_tree Suffix_tree.h Suffix_tree.cpp main.cpp)
