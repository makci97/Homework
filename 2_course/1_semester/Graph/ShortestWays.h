#pragma once
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <queue>
#include <tuple>
#include <vector>
#include "Graph.h"


template<typename T, typename W>
bool BellmanFord(Graph<T, W>& graph, const T& vertex, int amount_vertices);

template<typename T, typename W>
void FloydWarshall(Graph<T, W>& graph, int amount_vertices);

template<typename T, typename W>
void Dijkstra(Graph<T, W>& graph, const T& vertex, int amount_vertices);

template<typename T, typename W>
void ShortestWays()
{
/*
    std::ifstream is("test_D.txt");
    int flag, amount_edges, amount_vertices;
    T vertex;
    is >> flag;
    if(flag == 0 || flag == 2)
    {
        is >> vertex;
    }
    is >> amount_vertices >> amount_edges;

    Graph<int, int> graph;
    ReadGraph<int, int>(graph, amount_edges, is);
*/

    int flag, amount_edges, amount_vertices;
    T vertex;
    std::cin >> flag;
    if(flag == 0 || flag == 2)
    {
        std::cin >> vertex;
    }
    std::cin >> amount_vertices >> amount_edges;

    Graph<int, int> graph;

    ReadGraph<int, int>(graph, amount_edges, std::cin);

    if(flag == 0)
        BellmanFord(graph, vertex, amount_vertices);
    else if(flag == 1)
        FloydWarshall(graph, amount_vertices);
    else if(flag == 2)
        Dijkstra(graph, vertex, amount_vertices);

}


template<typename T, typename W>
bool BellmanFord(Graph<T, W>& graph, const T& vertex, int amount_vertices)
{
    std::map<T, W> shortest_ways;
    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        shortest_ways[*it] = INT_MAX;
    }

    if(shortest_ways[vertex] != INT_MAX)
        return false;

    shortest_ways[vertex] = 0;
    std::vector<typename Graph<T, W>::Edge> edges = graph.GetAllEdges();

    for(int i = 0; i < amount_vertices - 1; ++i)
    {
        for_each(edges.begin(), edges.end(), [&](const typename Graph<T, W>::Edge& edge)
        {
            if(shortest_ways[edge._first] != INT_MAX)
                shortest_ways[edge._second] = std::min(shortest_ways[edge._first] + edge._weight, shortest_ways[edge._second]);
        });
    }

    for(auto it = edges.begin(), end = edges.end();
        it != end;
        ++it)
    {
        if(shortest_ways[it -> _first] < shortest_ways[it -> _second] - it -> _weight)
        {\
            return false;\
        }
    }

    std::cout << "false" << std::endl;
    for_each(shortest_ways.begin(), shortest_ways.end(), [&](const std::pair<T, W>& way)
    {
        if(way.second == INT_MAX)
            std::cout << way.first << " -> " << "00" << std::endl;
        else
            std::cout << way.first << " -> " << way.second << std::endl;
    });

    return true;
}


template<typename T, typename W>
void FloydWarshall(Graph<T, W>& graph, int amount_vertices)
{
    Graph<T, W> shortest_ways = graph;

    for(auto it_short = shortest_ways.Begin(), end_short = shortest_ways.End();
        it_short != end_short;
        ++it_short)
    {
        shortest_ways.AddEdge(*it_short, *it_short, 0);

        for(auto it = shortest_ways.Begin(), end = shortest_ways.End();
            it != end;
            ++it)
        {
            shortest_ways.AddEdge(*it_short, *it, INT_MAX);
        }
    }

    std::vector<typename Graph<T, W>::Edge> edges = shortest_ways.GetAllEdges();


    for(auto first_it = shortest_ways.Begin(), first_end = shortest_ways.End();
        first_it != first_end;
        ++first_it)
    {

        for(auto second_it = shortest_ways.Begin(), second_end = shortest_ways.End();
            second_it != second_end;
            ++second_it)
        {
            W weight21 = shortest_ways.GetWeight(*second_it, *first_it);

            for(auto third_it = shortest_ways.Begin(), third_end = shortest_ways.End();
                third_it != third_end;
                ++third_it)
            {
                W& weight23 = shortest_ways.GetWeight(*second_it, *third_it);
                W weight13 = shortest_ways.GetWeight(*first_it, *third_it);

                if(weight21 != INT_MAX && weight13 != INT_MAX)
                {
                    weight23 = std::min(weight23, weight21 + weight13);
                }
            }

        }

    }

    shortest_ways.Print();
}

/*
template<typename T, typename W,
         template<typename Key, typename E, class Compare = std::greater<Key>, class Alloc = std::allocator<std::pair<const Key, E > >
         class Container// = std::multimap
         >
*/
template<typename T, typename W>
void Dijkstra(Graph<T, W>& graph, const T& vertex, int amount_vertices)
{
    std::map<T, W> shortest_ways;
    std::multimap<W, T> queue;
    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        shortest_ways[*it] = INT_MAX;
        if(*it != vertex)
            queue.insert({INT_MAX,*it});
        else
            queue.insert({0,*it});
    }

    if(shortest_ways[vertex] == INT_MAX)
    {

        shortest_ways[vertex] = 0;


        for(int i = 0; i < amount_vertices - 1; ++i)
        {
            while(!queue.empty())
            {
                T v = queue.begin() -> second;
                queue.erase(queue.begin());

                std::vector<typename Graph<T, W>::Edge> neighbors = graph.GetEdgesOfVertrex(v);

                for_each(neighbors.begin(), neighbors.end(), [&](const typename Graph<T, W>::Edge& edge)
                {
                    if(shortest_ways[edge._first] != INT_MAX)
                    {
                        if(shortest_ways[edge._first] + edge._weight < shortest_ways[edge._second])
                        {
                            shortest_ways[edge._second] = shortest_ways[edge._first] + edge._weight;

                            for(auto it = queue.begin(), end = queue.end();
                                it != end;
                                ++it)
                            {
                                if(it -> second == edge._second)
                                {
                                    queue.erase(it);
                                    queue.insert({shortest_ways[edge._second], edge._second});
                                    break;
                                }
                            }

                        }
                    }
                });

            }


        }

        for_each(shortest_ways.begin(), shortest_ways.end(), [&](const std::pair<T, W>& way)
        {
            if(way.second == INT_MAX)
                std::cout << way.first << " -> " << "00" << std::endl;
            else
                std::cout << way.first << " -> " << way.second << std::endl;
        });

    }
}

