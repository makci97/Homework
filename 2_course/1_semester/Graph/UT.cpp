#define BOOST_TEST_MODULE Read_graph
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <fstream>
#include "Graph.h"
#include "SCC/SCC.h"

BOOST_AUTO_TEST_CASE(Read_graph)
{
    std::ifstream is("test_graph.txt");
    int flag, amount_edges, amount_vertices;
    is >> flag;
    is >> amount_vertices >> amount_edges;

    /*std::cout << flag << std::endl;
    std::cout << amount_edges <<std::endl;
    std::cout << amount_vertices << std::endl;
    */

    Graph<int, int> graph;
    //ReadGraph<int, int>(graph, amount_edges, is);

    //SCC<int, int>();
}
