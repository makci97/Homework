cmake_minimum_required(VERSION 2.8)

project(TopResult) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC TopResult.cpp)

add_executable(TopResult ${SRC})
