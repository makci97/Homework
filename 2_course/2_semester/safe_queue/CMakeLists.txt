cmake_minimum_required(VERSION 2.8)

project(safe_queue) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(safe_queue safe_queue.h main.cpp)
