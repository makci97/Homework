#pragma once
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <vector>
#include <utility>
#include <stdexcept>

template<typename T>
class Graph
{
public:
    struct Edge;
    Graph(){}
    Graph(const std::vector<std::pair<T, T> >& graph);
    Graph(const Graph& graph):_graph(graph._graph) {}

    Graph& operator=(const Graph& other);
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;

    void Transp();
    void AddVertex(const T& vertex);
    void AddEdge(const T& first_vertex, const T& second_vertex);
    std::vector<Graph::Edge> GetEdgesOfVertrix(const T& vertex);
    std::vector<T> GetVertexesOfEdgesOfVertrix(const T& vertex);
    std::vector<Graph::Edge> GetEdgesEnteringInVertrix(const T& vertex);
    std::vector<T> GetVertexesOfEdgesEnteringInVertrix(const T& vertex);
    std::vector<Graph::Edge> GetAllEdges();
    std::vector<T> GetAllVertexes();

    typename std::set<T>::iterator GetBeginOfNeighbors(const T& vertex) const;
    typename std::set<T>::iterator GetEndOfNeighbors(const T& vertex) const;

    class DepthIterator
    {
    typedef typename std::map<T, std::set<T> >::iterator Iterator;
    public:
        DepthIterator() {}
        DepthIterator(Graph& graph);
        DepthIterator& operator=(const typename Graph::DepthIterator& dit);
        DepthIterator& operator++();
        DepthIterator& operator--();

        bool operator==(const typename Graph::DepthIterator& dit) const;
        bool operator!=(const typename Graph::DepthIterator& dit) const;
        T& operator*();

        void SetDit(const Iterator& dit);
        void SetStack(const typename std::stack<Iterator>& st);
        void SetVisited(const typename std::set<T>& visited);

    private:
        Graph<T>* _gr;
        Iterator _dit;
        typename std::stack<Iterator> _stack;
        typename std::set<T> _visited;
        typename std::list<std::pair<std::stack<Iterator>, T > > _change_graph; // стэк итераторов, которые были в _stack,
        //когда подграф закончился и элемент, на который перешел итератор после ++
    };

    DepthIterator Dbegin();
    DepthIterator Dend();


    struct Edge
    {
        T _first;
        T _second;
        bool operator==(const Edge& other);
        bool operator!=(const Edge& other);
    };

private:
    std::map<T, std::set<T> > _graph;
};

template<typename T>
std::ostream& operator << (std::ostream &stream, const typename Graph<T>::Edge& edge);
//*********************************************************************************
//Class Graph
//*********************************************************************************

template<typename T>
Graph<T>::Graph(const std::vector<std::pair<T, T> >& graph)
{
    for(auto it_vec = graph.begin(), end_vec = graph.end(); it_vec != end_vec; ++it_vec)
    {
        auto it_graph = _graph.find(it_vec -> first);
        if( it_graph != _graph.end() )
        {
            it_graph -> second.insert(it_vec -> second);
        }
        else
        {
            std::set<T> buf;
            buf.insert(it_vec -> second);
            _graph[it_vec -> first] = buf;
        }

        if(_graph.find(it_vec -> second) == _graph.end())
        {
            std::set<T> buf;
            _graph[it_vec -> second] = buf;
        }

    }
}

template<typename T>
void Graph<T>::Transp()
{
    Graph<T> buf_graph;

    auto it_graph = _graph.begin(), end_graph = _graph.end();

    while( it_graph != end_graph )
    {
        auto it_set_graph = it_graph -> second.begin(), end_set_graph = it_graph -> second.end();
        std::set<T> buf;

        if(buf_graph._graph.find(it_graph -> first) == buf_graph._graph.end())
            buf_graph._graph[it_graph -> first] = buf;

        buf.insert(it_graph -> first);

        while( it_set_graph != end_set_graph )
        {
            auto it_for_push = buf_graph._graph.find(*it_set_graph);
            if( it_for_push != buf_graph._graph.end() )
            {
                it_for_push -> second.insert(it_graph -> first);
            }
            else
            {
                buf_graph._graph[*it_set_graph] = buf;
            }

            ++it_set_graph;
        }

        ++it_graph;
    }

    *this = buf_graph;
}

template<typename T>
void Graph<T>::AddVertex(const T& vertex)
{
    if( _graph.find(vertex) == _graph.end() )
    {
        std::set<T> buf;
        _graph[vertex] = buf;
    }
}

template<typename T>
void Graph<T>::AddEdge(const T& first_vertex, const T& second_vertex)
{
    auto it = _graph.find(first_vertex);
    if( it != _graph.end() )
    {
        it -> second.insert(second_vertex);
    }
    else
    {
        std::set<T> buf;
        buf.insert(second_vertex);
        _graph[first_vertex] = buf;
    }

    if(_graph.find(second_vertex) == _graph.end())
    {
        std::set<T> buffer;
        _graph[second_vertex] = buffer;
    }

}

template<typename T>
typename std::vector<typename Graph<T>::Edge> Graph<T>::GetEdgesOfVertrix(const T& vertex)
{
    auto it_graph = _graph.find(vertex);
    std::vector<typename Graph<T>::Edge> result;

    if(it_graph != _graph.end())
    {
        auto it_set = it_graph -> second.begin(), end_set = it_graph -> second.end();

        typename Graph<T>::Edge buf;
        buf._first = vertex;

        while(it_set != end_set)
        {
            buf._second = *it_set;
            result.push_back(buf);
            ++it_set;
        }
    }

    return result;
}

template<typename T>
std::vector<T> Graph<T>::GetVertexesOfEdgesOfVertrix(const T& vertex)
{
    std::vector<T> result;

    auto it_graph = _graph.find(vertex);
    if(it_graph != _graph.end())
    {
        auto it_set = it_graph -> second.begin(), end_set = it_graph -> second.end();

        while(it_set != end_set)
        {
            result.push_back(*it_set);
            ++it_set;
        }
    }

    return result;
}

template<typename T>
typename std::vector<typename Graph<T>::Edge> Graph<T>::GetEdgesEnteringInVertrix(const T& vertex)
{
    auto it_this = _graph.find(vertex);
    std::vector<typename Graph<T>::Edge> result;

    if(it_this != _graph.end())
    {
        auto it_graph = _graph.begin(), end_graph = _graph.end();
        typename Graph<T>::Edge buf;
        buf._second = it_this -> first;

        while(it_graph != end_graph)
        {
            if(it_graph -> second.find(it_this -> first) != it_graph -> second.end())
            {
                buf._first = it_graph -> first;
                result.push_back(buf);
            }
            ++it_graph;
        }
    }

    return result;
}

template<typename T>
std::vector<T> Graph<T>::GetVertexesOfEdgesEnteringInVertrix(const T& vertex)
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


template<typename T>
typename std::vector<typename Graph<T>::Edge> Graph<T>::GetAllEdges()
{
    auto it_graph = _graph.begin(), end_graph = _graph.end();
    std::vector<typename Graph<T>::Edge> result;

    while(it_graph != end_graph)
    {
        auto it_set = it_graph -> second.begin(), end_set = it_graph -> second.end();
        typename Graph<T>::Edge buf;
        buf._first = it_graph -> first;

        while(it_set != end_set)
        {
            buf._second = *it_set;
            result.push_back(buf);

            ++it_set;
        }

        ++it_graph;
    }

    return result;
}

template<typename T>
std::vector<T> Graph<T>::GetAllVertexes()
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

template<typename T>
typename std::set<T>::iterator Graph<T>::GetBeginOfNeighbors(const T& vertex) const
{
    auto it = _graph.find(vertex);
    if(it != _graph.end())
        return it -> second.begin();
}

template<typename T>
typename std::set<T>::iterator Graph<T>::GetEndOfNeighbors(const T& vertex) const
{
    auto it = _graph.find(vertex);
    if(it != _graph.end())
        return it -> second.end();
}

template<typename T>
typename Graph<T>::DepthIterator Graph<T>::Dbegin()
{
    Graph<T>::DepthIterator new_it(*this);
    new_it.SetDit(_graph.begin());

    typename std::stack<typename std::map<T, std::set<T> >::iterator> st;
    st.push(_graph.begin());
    new_it.SetStack(st);
    typename std::set<T> visited;
    visited.insert(_graph.begin() -> first);
    new_it.SetVisited(visited);

    return new_it;
}

template<typename T>
typename Graph<T>::DepthIterator Graph<T>::Dend()
{
    Graph<T>::DepthIterator new_it(*this);
    new_it.SetDit(_graph.end());

    return new_it;
}

template<typename T>
Graph<T>& Graph<T>::operator=(const Graph& other)
{
    _graph = other._graph;
}

template<typename T>
bool Graph<T>::operator==(const Graph& other) const
{
    return _graph == other._graph;
}

template<typename T>
bool Graph<T>::operator!=(const Graph& other) const
{
    return _graph != other._graph;
}


//*********************************************************************************
//Class DepthIterator
//*********************************************************************************

template<typename T>
Graph<T>::DepthIterator::DepthIterator(Graph<T>& graph)
{
    _gr = &graph;
}

template<typename T>
typename Graph<T>::DepthIterator::DepthIterator& Graph<T>::DepthIterator::operator=(const typename Graph::DepthIterator& dit)
{
    _gr = dit._gr;
    _dit = dit._dit;
    _stack = dit._stack;
    _visited = dit._visited;
    _change_graph = dit._change_graph;
    return *this;
}

template<typename T>
typename Graph<T>::DepthIterator::DepthIterator& Graph<T>::DepthIterator::operator++()
{
    bool test = true;
    typename std::stack<Iterator> buf = _stack;

    while( !_stack.empty() && test && _dit != _gr -> _graph.end())
    {
        auto it_set_this = _dit -> second.begin(), end_set_this = _dit -> second.end();
        auto end_set_visited = _visited.end();
        while(it_set_this != end_set_this && _visited.find(*it_set_this) != end_set_visited)
            ++it_set_this;

        if(it_set_this == end_set_this)
        {
            _stack.pop();

            if( !_stack.empty() )
            {
                _dit = _stack.top();
            }
        }
        else    //if(_visited.find(*it_set_this) == end_set_visited)
        {
            _dit = _gr -> _graph.find(*it_set_this);
            _stack.push(_dit);
            _visited.insert(_dit -> first);
            test = false;
        }

    }

    if(test && _visited.size() == _gr -> _graph.size())
    {
        *this = _gr -> Dend();
    }

    else if( test && _visited.size() != _gr -> _graph.size() && _dit != _gr -> _graph.end())
    {
        //std::cout << "dohel" << std::endl;
        auto it_visited = _visited.begin(), end_visited = _visited.end();
        auto it_graph = _gr -> _graph.begin(), end_graph = _gr -> _graph.end();
        while(it_visited != end_visited && it_graph != end_graph && *it_visited == it_graph -> first)
        {
            ++it_visited;
            ++it_graph;
        }

        if(it_graph == end_graph)
        {
            *this = _gr -> Dend();
        }
        else
        {
            _dit = it_graph;
            _stack.push(_dit);
            _change_graph.push_back({buf, _dit -> first});
            _visited.insert(_dit -> first);

        }
    }


    //std::cout << "visited.size = " << _visited.size() << "; graph.size = " << _gr -> _graph.size();
    //std::cout << "; test = " << test << std::endl;
    return *this;
}


template<typename T>
typename Graph<T>::DepthIterator::DepthIterator& Graph<T>::DepthIterator::operator--()
{
    if(*this != _gr -> Dbegin() && *this != _gr -> Dend())
    {
        _visited.erase(_dit -> first);
        _stack.pop();

        if(!_stack.empty())
        {
            auto it_this = _stack.top() -> second.find(_dit -> first);
            if( it_this != _stack.top() -> second.begin() )
            {
                --it_this;
                _dit = _gr -> _graph.find(*it_this);
                _stack.push(_dit);
            }
            else
            {
                _dit = _stack.top();
            }
        }

        else
        {
            _stack = _change_graph.back().first;
            _change_graph.pop_back();
            _dit = _stack.top();
        }

    }

    return *this;
}

template<typename T>
bool Graph<T>::DepthIterator::operator==(const typename Graph<T>::DepthIterator& dit) const
{
    return (_gr == dit._gr) && (_dit == dit._dit) && (_stack == dit._stack) &&
           (_visited == dit._visited) && (_change_graph == dit._change_graph);

}

template<typename T>
bool Graph<T>::DepthIterator::operator!=(const typename Graph<T>::DepthIterator& dit) const
{
    return !( *this == dit);
}

template<typename T>
T& Graph<T>::DepthIterator::operator*()
{
    return const_cast<T&>(_dit -> first);
}



template<typename T>
void Graph<T>::DepthIterator::SetDit(const Iterator& dit)
{
    _dit = dit;
}

template<typename T>
void Graph<T>::DepthIterator::SetStack(const typename std::stack<Iterator>& st)
{
    _stack = st;
}

template<typename T>
void Graph<T>::DepthIterator::SetVisited(const typename std::set<T>& visited)
{
    _visited = visited;
}


//*********************************************************************************
//Struct Edge
//*********************************************************************************

template<typename T>
bool Graph<T>::Edge::operator==(const Graph<T>::Edge& other)
{
    return (_first == other._first) && (_second == other._second);
}

template<typename T>
bool Graph<T>::Edge::operator!=(const Graph<T>::Edge& other)
{
    return !(*this == other);
}

template<typename T>
std::ostream& operator << (std::ostream &stream, const typename Graph<T>::Edge& edge)
{
    stream << "{ "<< edge._first << "; "<< edge._second<< "}";
    return stream;
}
