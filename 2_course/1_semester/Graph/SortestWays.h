#pragma once
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <tuple>
#include <vector>
#include "Graph.h"


template<typename T, typename W>
bool BellmanFord(Graph<T, W>& graph, std::map<T, W>& sortest_ways, const T& vertex, int amount_vertices);


template<typename T, typename W>
void SortestWays()
{

    std::ifstream is("test_BF.txt");
    int flag, amount_edges, amount_vertices;
    T vertex;
    is >> flag;
    if(flag == 0)
    {
        is >> vertex;
        vertex = 11;
    }
    is >> amount_vertices >> amount_edges;

    Graph<int, int> graph;
    ReadGraph<int, int>(graph, amount_edges, is);

/*
    int flag, amount_edges, amount_vertices;
    T vertex;
    std::cin >> flag;
    if(flag == 0)
    {
        std::cin >> vertex;
    }
    std::cin >> amount_vertices >> amount_edges;

    Graph<int, int> graph;

    ReadGraph<int, int>(graph, amount_edges, std::cin);
*/
    std::map<T, W> sortest_ways;
    if(flag == 0)
        BellmanFord(graph, sortest_ways, vertex, amount_vertices);

}


template<typename T, typename W>
bool BellmanFord(Graph<T, W>& graph, std::map<T, W>& sortest_ways, const T& vertex, int amount_vertices)
{
    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        sortest_ways[*it] = INT_MAX;
    }

    if(sortest_ways[vertex] != INT_MAX)
        return false;

    sortest_ways[vertex] = 0;
    std::vector<typename Graph<T, W>::Edge> edges = graph.GetAllEdges();

    for(int i = 0; i < amount_vertices - 1; ++i)
    {
        for_each(edges.begin(), edges.end(), [&](const typename Graph<T, W>::Edge& edge)
        {
            if(sortest_ways[edge._first] != INT_MAX)
                sortest_ways[edge._second] = std::min(sortest_ways[edge._first] + edge._weight, sortest_ways[edge._second]);
        });
    }

    for(auto it = edges.begin(), end = edges.end();
        it != end;
        ++it)
    {
        if(sortest_ways[it -> _first] < sortest_ways[it -> _second] - it -> _weight)
        {\
            return false;\
        }
    }

    std::cout << "false" << std::endl;
    for_each(sortest_ways.begin(), sortest_ways.end(), [&](const std::pair<T, W>& way)
    {
        std::cout << way.first << " -> " << way.second << std::endl;
    });

    return true;
}
