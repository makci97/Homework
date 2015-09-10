#include <iostream>
#include <queue>
#include <fstream>
#include "Graph.h"


template<typename T>
void ReadGraph(Graph<T>& g, std::istream &in)
{
    int edges;

    if(!(in >> edges))
        throw std::logic_error("bad graph format: vertexes");

    if(!(in >> edges))
        throw std::logic_error("bad graph format: edges");

    for(int i = 0; i < edges; ++i)
    {
        T first;
        if(!(in >> first))
            throw std::logic_error("bad graph format: vertex");

        T second;
        if(!(in >> second))
            throw std::logic_error("bad graph format: vertex");

        g.AddEdge(first, second);
    }
}

template <typename T>
void DFSVisit(Graph<T>& g, const T& vertex, int& time, std::map<T, std::pair<int, int> >& schedule)
{
    schedule[vertex] = std::make_pair(time, 0);
    time++;

    for(auto it = g.GetBeginOfNeighbors(vertex), end = g.GetEndOfNeighbors(vertex);
        it != end;
        ++it)

    {
        if(!schedule.count(*it) && schedule[*it].second == 0)
        {
            DFSVisit(g, *it, time, schedule);
        }
    }

    schedule[vertex] = std::make_pair(schedule[vertex].first, time);
    time++;
}

template <typename T>
std::map<T, std::pair<int, int> > DFS(Graph<T>& g)
{
    std::map<T, std::pair<int, int> > output;
    int time = 1;


    typename Graph<T>::DepthIterator it = g.Dbegin(), end = g.Dend();

    for(; it != end; ++it)
    {
        if(!output.count(*it))
        {
            DFSVisit(g, *it, time, output);
        }
    }

    return output;
}

template <typename T>
void DFSVisit(const Graph<T>& g, const T& vertex, std::vector<T>& comp, std::map<T, bool>& used)
{
    used[vertex] = true;
    comp.push_back(vertex);
    for(auto it = g.GetBeginOfNeighbors(vertex), end = g.GetEndOfNeighbors(vertex);
         it != end;
         ++it)

    {
        if(!used[*it])
        {
            DFSVisit(g, *it, comp, used);
        }
    }
}

template <typename T>
std::vector<std::vector<T> > StronglyConnectedComponents(Graph<T>& g)
{
    std::vector<std::vector<T> > strongly_connected_components;

    std::map<T, std::pair<int, int> > schedule = DFS(g);
    Graph<T> transposed_graph = g;
    transposed_graph.Transp();

    std::map<T, bool> used;

    while(schedule.size())
    {
        std::vector<T> comp;

        int max = 0;
        typename std::map<T, std::pair<int, int> >::const_iterator it_max = schedule.begin();

        for(typename std::map<T, std::pair<int, int> >::const_iterator it_s = schedule.begin(), end_s = schedule.end();
             it_s != end_s;
             it_s++)

        {
            if(max < it_s -> second.second)
            {
                it_max = it_s;
                max = it_s -> second.second;
            }
        }

        if(!used[it_max -> first])
        {
            DFSVisit(transposed_graph, it_max -> first, comp, used);
        }

        if(comp.size() > 1)
        {
            strongly_connected_components.push_back(comp);
        }
        else if(comp.size() == 1)
        {
            for(auto it_n = g.GetBeginOfNeighbors(it_max->first);
                it_n != g.GetEndOfNeighbors(it_max->first);
                ++it_n)

            {
                if(*it_n == it_max -> first)
                {
                    strongly_connected_components.push_back(comp);
                    break;
                }
            }
        }

        schedule.erase(it_max);
    }
    return strongly_connected_components;
}

int main()
{
    Graph<std::string> g;

    ReadGraph(g, std::cin);

    std::vector<std::vector<std::string> > strongly_connected_components = StronglyConnectedComponents(g);
    std::set<std::string> classes;

    for(std::vector<std::vector<std::string> >::const_iterator it = strongly_connected_components.begin();
        it != strongly_connected_components.end();
        it++)

    {
        for(std::vector<std::string>::const_iterator it_s = it->begin();
            it_s != it->end();
            it_s++)

        {
            classes.insert(*it_s);
        }
    }

    std::ofstream out("output.txt");
    for(std::set<std::string>::const_iterator it_set = classes.begin();
        it_set != classes.end();
        it_set++)

    {
        out << *it_set << std::endl;
    }

    return 0;
}
