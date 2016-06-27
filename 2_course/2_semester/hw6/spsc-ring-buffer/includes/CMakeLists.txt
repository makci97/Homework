cmake_minimum_required(VERSION 2.8)

project(hw6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DCMAKE_BUILD_TYPE=debug")

add_executable(hw6 spsc_ring_buffer.h test_utils.h ya_contest_sim.h main.cpp)
