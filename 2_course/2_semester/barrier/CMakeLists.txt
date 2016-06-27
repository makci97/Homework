cmake_minimum_required(VERSION 2.8)

project(barrier)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(barrier barrier.h main.cpp)
