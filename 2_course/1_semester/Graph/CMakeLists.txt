cmake_minimum_required(VERSION 2.8)

project(Graph2) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Graph2 Graph.h SCC.h Run.h ShortestWays.h main.cpp)

project(UT) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(UT Graph.h SCC.h ShortestWays.h UT.cpp)

