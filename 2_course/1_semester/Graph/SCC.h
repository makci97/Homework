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

enum Color { white, grey, black};

template<typename T, typename W>
void DFSVisit(Graph<T, W>& graph, T vertex, unsigned int& time,
              std::map<T, Color>& color,
              std::map<T, T>& parent,
              std::map<T, int>& discover_time,
              std::map<T, int>& exit_time,
              std::map<T, int>& timestamp);

template<typename T, typename W>
void Kosaraju(Graph<T, W>& graph);

template<typename T, typename W>
void SCC()
{
    std::ifstream is("test_graph.txt");
    int flag, amount_edges, amount_vertices;
    is >> flag;
    is >> amount_vertices >> amount_edges;

    Graph<int, int> graph;
    ReadGraph<int, int>(graph, amount_edges, is);

    /*
    int flag, amount_edges, amount_vertices;
    std::cin >> flag;
    std::cin >> amount_vertices >> amount_edges;

    Graph<int, int> graph;

    ReadGraph<int, int>(graph, amount_edges, std::cin);
    */

    std::vector<std::list<T> > SCC;
    //if(flag == 0)
        //Tarian(graph, n, m);

    /*else*/ if(flag == 1)
        Kosaraju(graph, SCC);

    std::cout << SCC.size() << std::endl;

    for(auto it_SCC = SCC.begin(), end_SCC = SCC.end();
        it_SCC != end_SCC;
        ++it_SCC)
    {
        for(auto it = it_SCC -> begin(), end = it_SCC -> end();
            it != end;
            ++it)
        {
            std::cout << *it << "\t";
        }
        std::cout << "\n";
    }

}


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

    DFS<T, W>(transp_graph, DFS_exit, timestamp);

    while(!DFS_exit.empty())
    {
        /*
        std::cout << "DFS_exit: ";
        for(auto it = DFS_exit.begin(), end = DFS_exit.end();
            it != end;
            ++it)
        {
            std::cout << *it << "\t";
        }
        std::cout << "" << std::endl;
        */

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

        DFSVisit<T, W>(graph, vertex, time, color, parent, discover_time, exit_time, timestamp, SCC_buf);

        //std::cout << "SCC_buf: ";
        for(auto it = SCC_buf.begin(), end = SCC_buf.end();
            it != end;
            ++it)
        {
            graph.DeleteVertex(*it);
            //std::cout << *it << "\t";
        }
        //std::cout << "" << std::endl;

        for(auto it = SCC_buf.begin(), end = SCC_buf.end();
            it != end;
            ++it)
        {
            //std::cout << "ok " << *it << std::endl;
            DFS_exit.erase(std::find(DFS_exit.begin(), DFS_exit.end(), *it) );

            //std::cout << "?" << std::endl;
        }
        //std::cout << "okay" << std::endl;

        SCC_buf.sort();
        SCC.push_back(SCC_buf);
    }

    std::sort(SCC.begin(), SCC.end(), [&](std::list<T> first, std::list<T> second)
    {
        if(first.size() < second.size() )
            return true;
        else if(first.size() > second.size() )
            return false;
        else
        {
            return (first.front() < second.front() );
        }
    });
}
