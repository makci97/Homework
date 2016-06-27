cmake_minimum_required(VERSION 2.8)

project(robot)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(robot robot.cpp)
