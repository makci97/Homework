#pragma once
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <tuple>
#include <vector>
#include "../Graph.h"


enum Color { white, grey, black};


template<typename T, typename W>
void DFSVisit(Graph<T, W>& graph, T vertex, unsigned int& time,
              std::map<T, Color>& color,
              std::map<T, T>& parent,
              std::map<T, int>& discover_time,
              std::map<T, int>& exit_time,
              std::map<T, int>& timestamp);


template<typename T, typename W>
void Kosaraju(Graph<T, W>& graph, std::vector<std::list<T> >& SCC);


template<typename T, typename W>
void Tarian(Graph<T, W>& graph, std::vector<std::list<T> >& SCC);


//DFS_exit for Kosaraju, timestamp for Tarian
template<typename T, typename W>
void DFS(Graph<T, W>& graph, std::list<T>& DFS_exit, std::map<T, unsigned int>& timestamp)
{
    std::map<T, Color> color;
    std::map<T, T> parent;
    std::map<T, unsigned int> discover_time;
    std::map<T, unsigned int> exit_time;

    for(auto it = graph.Begin(), end = graph.End();
        it != end;
        ++it)
    {
        color[*it] = white;
        parent[*it] = *it;
        discover_time[*it] = INT_MAX;
        exit_time[*it] = INT_MAX;
        timestamp[*it] = INT_MAX;
    }

    unsigned int time = 0;

    for_each(color.begin(), color.end(), [&](std::pair<T, Color> node)
    {
        if(node.second == white)
            DFSVisit<T, W>(graph, node.first, time, color, parent, discover_time, exit_time, timestamp, DFS_exit);
    });
}


template<typename T, typename W>
void DFSVisit(Graph<T, W>& graph, T vertex, unsigned int& time,
              std::map<T, Color>& color,
              std::map<T, T>& parent,
              std::map<T, unsigned int>& discover_time,
              std::map<T, unsigned int>& exit_time,
              std::map<T, unsigned int>& timestamp,
              std::list<T>& DFS_exit)

{
    ++time;
    discover_time[vertex] = time;
    timestamp[vertex] = time;
    color[vertex] = grey;
    std::vector<T> neighbors = graph.GetVerticesOfEdgesOfVertrex(vertex);

    for_each(neighbors.begin(), neighbors.end(), [&](T v)
    {
       if(color[v] == white)
       {
           parent[v] = vertex;
           DFSVisit<T, W>(graph, v, time, color, parent, discover_time, exit_time, timestamp, DFS_exit);
       }

       if(timestamp[v] < timestamp[vertex])
           timestamp[vertex] = timestamp[v];

    });

    color[vertex] = black;
    ++time;
    exit_time[vertex] = time;
    DFS_exit.push_back(vertex);
}


template<typename T, typename W>
void Kosaraju(Graph<T, W>& graph, std::vector<std::list<T> >& SCC)
{
    Graph<T, W> transp_graph = graph;
    transp_graph.Transp();
    std::list<T> DFS_exit;
    std::map<T, unsigned int> timestamp;

    DFS<T, W>(graph, DFS_exit, timestamp);

    while(!DFS_exit.empty())
    {
        T vertex = DFS_exit.back();

        std::map<T, Color> color;

        for(auto it = graph.Begin(), end = graph.End();
            it != end;
            ++it)
        {
            color[*it] = white;
        }

        std::map<T, T> parent;
        std::map<T, unsigned int> discover_time;
        std::map<T, unsigned int> exit_time;
        unsigned int time;

        std::list<T> SCC_buf;

        DFSVisit<T, W>(transp_graph, vertex, time, color, parent, discover_time, exit_time, timestamp, SCC_buf);

        for(auto it = SCC_buf.begin(), end = SCC_buf.end();
            it != end;
            ++it)
        {
            transp_graph.DeleteVertex(*it);
        }

        for(auto it = SCC_buf.begin(), end = SCC_buf.end();
            it != end;
            ++it)
        {
            DFS_exit.erase(std::find(DFS_exit.begin(), DFS_exit.end(), *it) );
        }

        SCC_buf.sort();
        SCC.push_back(SCC_buf);
    }
}


template<typename T, typename W>
void Tarian(Graph<T, W>& graph, std::vector<std::list<T> >& SCC)
{
    std::list<T> DFS_exit;
    std::map<T, unsigned int> timestamp;
    DFS(graph, DFS_exit, timestamp);

    std::multimap<unsigned int, T> for_sort;

    for(auto it = timestamp.begin(), end = timestamp.end();
        it != end;
        ++it)
    {
        for_sort.insert({it -> second, it -> first});
    }

    for(auto it = for_sort.begin(), end = for_sort.end();
        it != end;)
    {
        unsigned int stamp = it -> first;
        std::list<T> component_connectivity;

        while(it != end && it -> first == stamp)
        {
            component_connectivity.push_back(it -> second);
            ++it;
        }

        component_connectivity.sort();
        SCC.push_back(component_connectivity);
    }
}
