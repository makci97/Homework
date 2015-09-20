#pragma once
#include <climits>
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>

template<typename T, typename W>
class Graph
{
public:
    struct Edge;
    Graph(){}
    Graph(const std::vector<std::tuple<T, T, W> >& graph);
    Graph(const std::vector<Graph::Edge>& graph);
    Graph(const Graph& graph):_graph(graph._graph) {}

    Graph& operator=(const Graph& other);
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;

    void Transp();
    void AddVertex(const T& vertex);
    void DeleteVertex(const T& vertex);
    void AddEdge(const T& first_vertex, const T& second_vertex, const W& weight);
    void AddEdge(const Graph::Edge& edge);
    std::vector<Graph::Edge> GetEdgesOfVertrex(const T& vertex);
    std::vector<T> GetVerticesOfEdgesOfVertrex(const T& vertex);
    std::vector<Graph::Edge> GetEdgesEnteringInVertrex(const T& vertex);
    std::vector<T> GetVerticesOfEdgesEnteringInVertrex(const T& vertex);
    std::vector<Graph::Edge> GetAllEdges();
    std::vector<T> GetAllVertices();
    W& GetWeight(const T& first, const T& second);
    void Print();

    typename std::map<T, W>::iterator GetBeginOfNeighbors(const T& vertex) const;
    typename std::map<T, W>::iterator GetEndOfNeighbors(const T& vertex) const;

    class Iterator
    {
    typedef typename std::map<T, std::map<T, W> >::iterator TIterator;
    public:
        Iterator() {}
        Iterator(Graph& graph);
        Iterator& operator=(const typename Graph::Iterator& dit);
        Iterator& operator++();
        Iterator& operator--();

        bool operator==(const typename Graph::Iterator& dit) const;
        bool operator!=(const typename Graph::Iterator& dit) const;
        T& operator*();

        void SetDit(const TIterator& dit);


    private:
        Graph<T, W>* _gr;
        TIterator _dit;
    };


    Iterator Begin();
    Iterator End();


    struct Edge
    {
        T _first;
        T _second;
        W _weight;
        Edge(){}
        Edge(const T& first, const T& second, const W& weight):
            _first(first), _second(second), _weight(weight){}

        bool operator==(const Edge& other);
        bool operator!=(const Edge& other);
        void Print();
    };

private:
    std::map<T, std::map<T, W> > _graph;
};

template<typename T, typename W>
std::ostream& operator << (std::ostream &stream, const typename Graph<T, W>::Edge& edge);

template<typename T, typename W>
std::ostream& operator << (std::ostream &stream, Graph<T, W>& graph);
//*********************************************************************************
//Class Graph
//*********************************************************************************


template<typename T, typename W>
Graph<T, W>::Graph(const std::vector<Graph::Edge>& graph)
{
    for(auto it_vec = graph.begin(), end_vec = graph.end(); it_vec != end_vec; ++it_vec)
    {
        auto it_graph = _graph.find(it_vec -> _first);
        if( it_graph != _graph.end() )
        {
            it_graph -> second.insert({it_vec -> _second, it_vec -> _weight});
        }
        else
        {
            std::map<T, W> buf;
            buf.insert({it_vec -> _second, it_vec -> _weight});
            _graph[it_vec -> _first] = buf;
        }

        if(_graph.find(it_vec -> _second) == _graph.end())
        {
            std::map<T, W> buf;
            _graph[it_vec -> _second] = buf;
        }

    }
}


template<typename T, typename W>
Graph<T, W>::Graph(const std::vector<std::tuple<T, T, W> >& graph)
{
    for(auto it_vec = graph.begin(), end_vec = graph.end(); it_vec != end_vec; ++it_vec)
    {
        auto it_graph = _graph.find(std::get<0>(*it_vec));
        if( it_graph != _graph.end() )
        {
            it_graph -> second.insert({std::get<1>(*it_vec), std::get<2>(*it_vec)});
        }
        else
        {
            std::map<T, W> buf;
            buf.insert({std::get<1>(*it_vec), std::get<2>(*it_vec)});
            _graph[std::get<0>(*it_vec)] = buf;
        }

        if(_graph.find(std::get<1>(*it_vec)) == _graph.end())
        {
            std::map<T, W> buf;
            _graph[std::get<1>(*it_vec)] = buf;
        }

    }
}


template<typename T, typename W>
void Graph<T, W>::Transp()
{
    Graph<T, W> buf_graph;

    auto it_graph = _graph.begin(), end_graph = _graph.end();

    while( it_graph != end_graph )
    {
        auto it_map_graph = it_graph -> second.begin(), end_map_graph = it_graph -> second.end();
        std::map<T,W> buf;

        if(buf_graph._graph.find(it_graph -> first) == buf_graph._graph.end())
            buf_graph._graph[it_graph -> first] = buf;

       // buf.insert(it_graph -> first);

        while( it_map_graph != end_map_graph )
        {
            auto it_for_push = buf_graph._graph.find(it_map_graph -> first);
            if( it_for_push != buf_graph._graph.end() )
            {
                it_for_push -> second.insert({it_graph -> first, it_map_graph -> second});
            }
            else
            {
                std::map<T,W> buffer;
                buffer.insert({it_graph -> first, it_map_graph -> second});
                buf_graph._graph[it_map_graph -> first] = buffer;
            }

            ++it_map_graph;
        }

        ++it_graph;
    }

    *this = buf_graph;
}


template<typename T, typename W>
void Graph<T, W>::AddVertex(const T& vertex)
{
    if( _graph.find(vertex) == _graph.end() )
    {
        std::map<T, W> buf;
        _graph[vertex] = buf;
    }
}

template<typename T, typename W>
void Graph<T, W>::DeleteVertex(const T& vertex)
{
    if(_graph.find(vertex) != _graph.end() )
    {
        _graph.erase(_graph.find(vertex) );

        for(auto it = _graph.begin(), end = _graph.end();
            it != end;
            ++it)
        {
            if((it -> second).find(vertex) != (it -> second).end() )
                (it -> second).erase((it -> second).find(vertex));
        }
    }
}

template<typename T, typename W>
void Graph<T, W>::AddEdge(const T& first_vertex, const T& second_vertex, const W& weight)
{
    auto it = _graph.find(first_vertex);
    if( it != _graph.end() )
    {
        it -> second.insert({second_vertex, weight});
    }
    else
    {
        std::map<T, W> buf;
        buf.insert({second_vertex, weight});
        _graph[first_vertex] = buf;
    }

    if(_graph.find(second_vertex) == _graph.end())
    {
        std::map<T, W> buf;
        _graph[second_vertex] = buf;
    }
}


template<typename T, typename W>
void Graph<T, W>::AddEdge(const Graph<T,W>::Edge& edge)
{
    auto it = _graph.find(edge._first);
    if( it != _graph.end() )
    {
        it -> second.insert({edge._second, edge._weight});
    }
    else
    {
        std::map<T, W> buf;
        buf.insert({edge._second, edge._weight});
        _graph[edge._first] = buf;
    }

    if(_graph.find(edge._second) == _graph.end())
    {
        std::map<T, W> buf;
        _graph[edge._second] = buf;
    }

}
template<typename T, typename W>
typename std::vector<typename Graph<T, W>::Edge> Graph<T, W>::GetEdgesOfVertrex(const T& vertex)
{
    auto it_graph = _graph.find(vertex);
    std::vector<typename Graph<T, W>::Edge> result;

    if(it_graph != _graph.end())
    {
        auto it_map = it_graph -> second.begin(), end_map = it_graph -> second.end();

        typename Graph<T, W>::Edge buf;
        buf._first = vertex;

        while(it_map != end_map)
        {
            buf._second = it_map -> first;
            buf._weight = it_map -> second;
            result.push_back(buf);
            ++it_map;
        }
    }

    return result;
}


template<typename T, typename W>
std::vector<T> Graph<T, W>::GetVerticesOfEdgesOfVertrex(const T& vertex)
{
    std::vector<T> result;

    auto it_graph = _graph.find(vertex);
    if(it_graph != _graph.end())
    {
        auto it_map = it_graph -> second.begin(), end_map = it_graph -> second.end();

        while(it_map != end_map)
        {
            result.push_back(it_map -> first);
            ++it_map;
        }
    }

    return result;
}


template<typename T, typename W>
typename std::vector<typename Graph<T, W>::Edge> Graph<T, W>::GetEdgesEnteringInVertrex(const T& vertex)
{
    auto it_this = _graph.find(vertex);
    std::vector<typename Graph<T, W>::Edge> result;

    if(it_this != _graph.end())
    {
        auto it_graph = _graph.begin(), end_graph = _graph.end();
        typename Graph<T, W>::Edge buf;
        buf._second = it_this -> first;

        while(it_graph != end_graph)
        {
            auto it_map = it_graph -> second.find(it_this -> first), end_map = it_graph -> second.end();
            if(it_map != end_map)
            {
                buf._first = it_graph -> first;
                buf._weight = it_map -> second;
                result.push_back(buf);
            }
            ++it_graph;
        }
    }

    return result;
}


template<typename T, typename W>
std::vector<T> Graph<T, W>::GetVerticesOfEdgesEnteringInVertrex(const T& vertex)
{
    std::vector<T> result;

    auto it_this = _graph.find(vertex);

    if(it_this != _graph.end())
    {
        auto it_graph = _graph.begin(), end_graph = _graph.end();

        while(it_graph != end_graph)
        {
            if(it_graph -> second.find(it_this -> first) != it_graph -> second.end())
            {
                result.push_back(it_graph -> first);
            }
            ++it_graph;
        }
    }

    return result;
}


template<typename T, typename W>
typename std::vector<typename Graph<T, W>::Edge> Graph<T, W>::GetAllEdges()
{
    auto it_graph = _graph.begin(), end_graph = _graph.end();
    std::vector<typename Graph<T, W>::Edge> result;

    while(it_graph != end_graph)
    {
        auto it_map = it_graph -> second.begin(), end_map = it_graph -> second.end();
        typename Graph<T, W>::Edge buf;

        buf._first = it_graph -> first;

        while(it_map != end_map)
        {
            buf._second = it_map -> first;
            buf._weight = it_map -> second;
            result.push_back(buf);

            ++it_map;
        }

        ++it_graph;
    }

    return result;
}


template<typename T, typename W>
std::vector<T> Graph<T, W>::GetAllVertices()
{

    auto it_graph = _graph.begin(), end_graph = _graph.end();
    std::vector<T> result;

    while(it_graph != end_graph)
    {
        result.push_back(it_graph -> first);

        ++it_graph;
    }

    return result;
}

template<typename T, typename W>
W& Graph<T, W>::GetWeight(const T& first, const T& second)
{
    if(_graph.find(first) != _graph.end() )
        if(_graph.find(first) -> second.find(second) != _graph.find(first) -> second.end() )
        {
            return _graph.find(first) -> second.find(second) -> second;
        }
}


template<typename T, typename W>
void Graph<T, W>::Print()
{
    std::vector<typename Graph<T, W>::Edge> edges = this -> GetAllEdges();

    for(auto it = edges.begin(), end = edges.end();
        it != end;
        ++it)
    {
        it -> Print();
    }
}


template<typename T, typename W>
typename std::map<T, W>::iterator Graph<T, W>::GetBeginOfNeighbors(const T& vertex) const
{
    auto it = _graph.find(vertex);
    if(it != _graph.end())
        return it -> second.begin();
}


template<typename T, typename W>
typename std::map<T, W>::iterator Graph<T, W>::GetEndOfNeighbors(const T& vertex) const
{
    auto it = _graph.find(vertex);
    if(it != _graph.end())
        return it -> second.end();
}


template<typename T, typename W>
typename Graph<T, W>::Iterator Graph<T, W>::Begin()
{    
    Graph<T, W>::Iterator new_it(*this);
    new_it.SetDit(_graph.begin());

    return new_it;
}


template<typename T, typename W>
typename Graph<T, W>::Iterator Graph<T, W>::End()
{
    Graph<T, W>::Iterator new_it(*this);
    new_it.SetDit(_graph.end());

    return new_it;
}


template<typename T, typename W>
Graph<T, W>& Graph<T, W>::operator=(const Graph& other)
{
    _graph = other._graph;
}


template<typename T, typename W>
bool Graph<T, W>::operator==(const Graph& other) const
{
    return _graph == other._graph;
}


template<typename T, typename W>
bool Graph<T, W>::operator!=(const Graph& other) const
{
    return _graph != other._graph;
}


template<typename T, typename W>
std::ostream& operator << (std::ostream &stream, Graph<T, W>& graph)
{
    std::vector<typename Graph<T, W>::Edge> edges = graph.GetAllEdges();

    for(auto it = edges.begin(), end = edges.end();
        it != end;
        ++it)
    {
        //stream << *it << std::endl;
    }

    return stream;
}

//*********************************************************************************
//Class Iterator
//*********************************************************************************

template<typename T, typename W>
Graph<T, W>::Iterator::Iterator(Graph<T, W>& graph)
{
    _gr = &graph;
}


template<typename T, typename W>
typename Graph<T, W>::Iterator::Iterator&
         Graph<T, W>::Iterator::operator=(const typename Graph::Iterator& dit)

{
    _gr = dit._gr;
    _dit = dit._dit;
    return *this;
}


template<typename T, typename W>
typename Graph<T, W>::Iterator::Iterator&
         Graph<T, W>::Iterator::operator++()

{
    if(_dit != _gr->_graph.end())
        ++_dit;

    return *this;
}


template<typename T, typename W>
typename Graph<T, W>::Iterator::Iterator& Graph<T, W>::Iterator::operator--()
{
    if(_dit != _gr->_graph.begin())
        --_dit;

    return *this;
}

template<typename T, typename W>
bool Graph<T, W>::Iterator::operator==(const typename Graph<T, W>::Iterator& dit) const
{
    return (_gr == dit._gr) && (_dit == dit._dit);

}


template<typename T, typename W>
bool Graph<T, W>::Iterator::operator!=(const typename Graph<T, W>::Iterator& dit) const
{
    return !( *this == dit);
}


template<typename T, typename W>
T& Graph<T, W>::Iterator::operator*()
{
    return const_cast<T&>(_dit -> first);
}


template<typename T, typename W>
void Graph<T, W>::Iterator::SetDit(const TIterator& dit)
{
    _dit = dit;
}

//*********************************************************************************
//Struct Edge
//*********************************************************************************

template<typename T, typename W>
bool Graph<T, W>::Edge::operator==(const Graph<T, W>::Edge& other)
{
    return (_first == other._first)
            && (_second == other._second)
            && (_weight == other._weight);
}


template<typename T, typename W>
bool Graph<T, W>::Edge::operator!=(const Graph<T, W>::Edge& other)
{
    return !(*this == other);
}


template<typename T, typename W>
std::ostream& operator << (std::ostream &stream, const typename Graph<T, W>::Edge& edge)
{
    if(edge._weight != INT_MAX)
    {
        stream << "{ "<< edge._first << "; "
               << edge._second << "; "
               << edge._weight << "}";
    }
    else
        stream << "{ "<< edge._first << "; "
               << edge._second << "; "
               << "00}";

    return stream;
}


template<typename T, typename W>
void Graph<T, W>::Edge::Print()
{
    if(this -> _weight != INT_MAX)
    {
        std::cout << "{ "<< this -> _first << "; "
                  << this -> _second << "; "
                  << this -> _weight << "}";
    }
    else
        std::cout << "{ "<< this -> _first << "; "
                  << this -> _second << "; "
                  << "00}";
}

//*********************************************************************************

template<typename T, typename W>
bool ReadGraph(Graph<T,W>& g, int amount_edges, std::istream &in)
{
    for(int i = 0; i < amount_edges; ++i)
    {
        T first;
        if(!(in >> first))
            throw std::logic_error("bad graph format: vertex");

        T second;
        if(!(in >> second))
            throw std::logic_error("bad graph format: vertex");

        W weight;
        if(!(in >> weight))
            throw std::logic_error("bad graph format: weight");

        g.AddEdge(first, second, weight);
    }

    return true;
}
