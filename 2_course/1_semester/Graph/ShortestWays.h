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

template<typename T, typename W,
             template<
                 class P,
                 class Container = std::vector<P>,
                 class Compare = std::greater<typename Container::value_type>
             > class priority_queue
         >
void Dijkstra(Graph<T, W>& graph, const T& vertex, int amount_vertices);

template<typename T, typename W,
             template<
                 class P,
                 class Container = std::vector<P>,
                 class Compare = std::greater<typename Container::value_type>
             > class priority_queue
         >
void AStar(Graph<T, W>& graph, const T& vertex, const T& target, std::map<T, W> heuristic, int amount_vertices);

void Fifteen(std::vector<int>& start_position);

template<typename T, typename W>
void ReadHeuristic(std::map<T, W> heuristic, int amount_vertices, std::istream &in)
{
    T vertex;
    W weight_heuristic;
    for(int i = 0; i < amount_vertices; ++i)
    {
        if(!(in >> vertex))
            throw std::logic_error("bad graph format: vertex");

        if(!(in >> weight_heuristic))
            throw std::logic_error("bad graph format: vertex");

        heuristic[vertex] = weight_heuristic;
    }
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
        {
            std::cout << "-1" << std::endl;
            return false;
        }
    }


    for_each(shortest_ways.begin(), shortest_ways.end(), [&](const std::pair<T, W>& way)
    {
        if(way.second == INT_MAX)
            std::cout << way.first << " -> " << "-1" << std::endl;
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

    shortest_ways.PrintFW();
}

/*
template<typename T, typename W,
         template<typename Key, typename E, class Compare = std::greater<Key>, class Alloc = std::allocator<std::pair<const Key, E > >
         class Container// = std::multimap
         >
*/
/*
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


        for_each(shortest_ways.begin(), shortest_ways.end(), [&](const std::pair<T, W>& way)
        {
            if(way.second == INT_MAX)
                std::cout << way.first << " -> " << "-1" << std::endl;
            else
                std::cout << way.first << " -> " << way.second << std::endl;
        });

    }
}
*/

/*template<typename T,typename W>
class Comparator
{
    bool operator() (const std::pair<W, T>& first, const std::pair<W, T>& second)
    {
        return first.first > second.first;
    }
};*/

template<typename T, typename W,
             template<
                 class P,
                 class Container = std::vector<P>,
                 class Compare = std::greater<typename Container::value_type>
             > class priority_queue
         >
void Dijkstra(Graph<T, W>& graph, const T& vertex, int amount_vertices)
{
    std::map<T, W> shortest_ways; //(vertex, distanse, distanse + heuristic)
    std::map<T, bool> visit;
    priority_queue<std::pair<W, T> > queue;

    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        shortest_ways[*it] = INT_MAX;
        visit[*it] = false;
        if(*it != vertex)
            queue.push({INT_MAX,*it});
        else
            queue.push({0,*it});
    }

    int size = queue.size();

    shortest_ways[vertex] = 0;

    while(--size >= 0)
    {
        T v = queue.top().second;
        queue.pop();

        if(!visit[v])
        {

            std::vector<typename Graph<T, W>::Edge> neighbors = graph.GetEdgesOfVertrex(v);

            for_each(neighbors.begin(), neighbors.end(), [&](const typename Graph<T, W>::Edge& edge)
            {
                if(shortest_ways[edge._first] != INT_MAX)
                {
                    if(shortest_ways[edge._first] + edge._weight < shortest_ways[edge._second])
                    {
                        shortest_ways[edge._second] = shortest_ways[edge._first] + edge._weight;
                        queue.push({shortest_ways[edge._first] + edge._weight, edge._second});
                    }
                }
            });

            visit[v] = true;
        }

    }

    for_each(shortest_ways.begin(), shortest_ways.end(), [&](const std::pair<T, W>& way)
    {
        if(way.second == INT_MAX)
        {
            std::cout << "-1" << std::endl;
            //std::cout << way.first << " -> " << "-1" << std::endl;
        }
        else
        {
            std::cout << way.second << std::endl;
            //std::cout << way.first << " -> " << way.second << std::endl;
        }
    });
}

template<typename T, typename W,
             template<
                 class P,
                 class Container = std::vector<P>,
                 class Compare = std::greater<typename Container::value_type>
             > class priority_queue
         >
void AStar(Graph<T, W>& graph, const T& vertex, const T& target, std::map<T, W> heuristic, int amount_vertices)
{
    std::map<T, std::pair<W, W> > shortest_ways; //(vertex, distanse, distanse + heuristic)
    std::map<T, bool> visit;
    priority_queue<std::pair<W, T> > queue;

    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        shortest_ways[*it] = {INT_MAX, INT_MAX};
        visit[*it] = false;
        if(*it != vertex)
            queue.push({INT_MAX,*it});
        else
            queue.push({0,*it});
    }

    int size = queue.size();

    shortest_ways[vertex] = {0, 0};

    while(--size >= 0)
    {
        T v = queue.top() -> second;

        if(v == target)
        {
            break;
        }
        queue.pop();

        if(!visit[v])
        {

            std::vector<typename Graph<T, W>::Edge> neighbors = graph.GetEdgesOfVertrex(v);

            for_each(neighbors.begin(), neighbors.end(), [&](const typename Graph<T, W>::Edge& edge)
            {
                if(shortest_ways[edge._first] != std::make_pair(INT_MAX, INT_MAX) )
                {
                    if(shortest_ways[edge._first].first + edge._weight < shortest_ways[edge._second].first)
                    {
                        shortest_ways[edge._second] = {shortest_ways[edge._first].first + edge._weight,
                                                       shortest_ways[edge._first].first + edge._weight +
                                                       heuristic[edge._second]};

                        queue.push({shortest_ways[edge._first].first + edge._weight + heuristic[edge._second],
                                    edge._second});

                    }
                }
            });

            visit[v] = true;
        }

    }

    if(shortest_ways[target] == INT_MAX)
        std::cout << vertex << " -> " << target << " = -1" << std::endl;
    else
        std::cout << vertex << " -> " << target << " = " << shortest_ways[target] << std::endl;

}

void Fifteen(std::vector<int>& start_position)
{

}
