cmake_minimum_required(VERSION 2.8)

add_subdirectory(Fifteen)
add_subdirectory(Heaps)
add_subdirectory(SCC)
add_subdirectory(ShortestWays)
add_subdirectory(Minimum_spanning_tree)

project(Graph2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Graph2 Graph.h SCC/SCC.h Run.h Fifteen/Fifteen.h ShortestWays/ShortestWays.h  Minimum_spanning_tree/Minimum_spanning_tree.h main.cpp)


