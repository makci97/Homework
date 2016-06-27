cmake_minimum_required(VERSION 2.8)

project(spsc_ring_buffer)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DCMAKE_BUILD_TYPE=debug")

add_executable(spsc_ring_buffer spsc_ring_buffer.h main.cpp)
