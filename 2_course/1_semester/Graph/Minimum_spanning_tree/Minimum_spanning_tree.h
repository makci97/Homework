#pragma once
#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <vector>

class DSU
{
public:
    DSU(): _count(0) {}

    //add n sets from 0 to n-1
    DSU(int n);

    //add set with one element x if not belong other set
    inline void MakeSet(int x);

    //return delegate of set which x belong
    inline int Find(int x);

    //unite the sets which belong to the set elements x and y
    inline void Unite(int x, int y);

    //return number of sets
    inline int GetNumberOfSets();

private:
    std::vector<int> _p;    //parent's array
    int _count;             //number of sets
};

long int Kruskal_algorithm(int amount_vertices,
        const std::multimap<int, std::pair<int, int> >& graph);


//***************************************************************************************
//*******************************************************
//DSU
//*******************************************************

//add n sets from 0 to n-1
DSU::DSU(int n)
{
    _p.resize(n);
    _count = n;

    for(int i = 0; i < n; ++i)
        _p[i] = i;
}

//add set with one element x if not belong other set
void DSU::MakeSet(int x)
{
    if(_p.size() <= x)
        _p.resize(x + 1, -1);

    if(_p[x] == -1)
    {
        _p[x] = x;
        ++_count;
    }
}

//return delegate of set which x belong
int DSU::Find(int x)
{
    if(x >= _p.size())
        return -1;

    else if(_p[x] == x)
        return x;

    //heruistic of path compression
    return _p[x] = Find(_p[x]);
}

//unite the sets which belong to the set elements x and y
void DSU::Unite(int x, int y)
{
    x = Find(x);
    y = Find(y);

    if(x == -1 || y == -1 || x == y)
        return;

    --_count;

    std::random_device Rand;

    if(Rand() % 2 == 0)
        _p[x] = y;
    else
        _p[y] = x;
}

//return number of sets
int DSU::GetNumberOfSets()
{
    return _count;
}

//*******************************************************
//Kruskal_algorithm
//*******************************************************

long int Kruskal_algorithm(int amount_vertices,
        const std::multimap<int, std::pair<int, int> >& graph)
{
    DSU dsu(amount_vertices);

    long int result_weigth = 0;

    for(auto it = graph.begin(), end = graph.end();
        it != end; ++it)
    {
        if(dsu.Find(it->second.first) != dsu.Find(it->second.second))
        {
            //if from and to belong diferent sets
            dsu.Unite(dsu.Find(it->second.first),
                      dsu.Find(it->second.second));
            //unite this sets in one

            //std::cout << it->second.first << " " << it->second.second << std::endl;

            result_weigth += it->first;

            if(dsu.GetNumberOfSets() == 1)
            {
                //MST found
                return result_weigth;
            }
        }

    }

    assert(false);
}

