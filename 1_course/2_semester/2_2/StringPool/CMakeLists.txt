cmake_minimum_required(VERSION 2.8)

project(StringPool) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(StringPool StringPool.cpp main.cpp)
