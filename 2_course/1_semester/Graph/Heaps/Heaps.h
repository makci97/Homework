#pragma once
#include <iostream>
#include <list>
#include <math.h>
#include <memory>
#include <vector>


template<class T, class Container = std::vector<T>,
         class Compare = std::less<typename Container::value_type>
        >
struct binomial_tree
{
    typedef std::shared_ptr<binomial_tree> b_t_ptr;

    binomial_tree(const T& element);
    int size() const;
    int degree() const;
    T& top();
    
    static std::pair<bool, b_t_ptr> merge(b_t_ptr first, b_t_ptr second);

    T _data;
    b_t_ptr _parent;
    std::list<std::shared_ptr<binomial_tree> > _childreen;
};


template<class T, class Container = std::vector<T>,
         class Compare = std::less<typename Container::value_type>
        >
class binomial_heap
{
public:
    typedef T value_type;
    typedef binomial_tree<value_type, Container, Compare> b_t;
    typedef std::list<std::shared_ptr<b_t> > list_ptr;

    binomial_heap(): _min_it(_list_of_roots.end()){}

    bool empty() const;
    int size() const;       //amount of trees in heap
    int true_size() const;
    value_type top();
    void push(const value_type& new_elem);
    void pop();

    void merge(b_t other);
    static binomial_heap merge(binomial_heap first, binomial_heap second);
    bool decrease_key(typename b_t::b_t_ptr node, value_type key);
    bool delete_key(typename b_t::b_t_ptr node, value_type key);

private:
    list_ptr _list_of_roots;
    typename list_ptr::iterator _min_it;

    void refresh_min();
    void fast_merge(std::shared_ptr<b_t> tree_ptr);
    static binomial_heap& fast_merge(binomial_heap& first, binomial_heap& second);
};

//**********************************************************************
//binomial_tree
//**********************************************************************

template<class T, class Container, class Compare>
binomial_tree<T, Container, Compare>::binomial_tree(const T& element)
{
    _parent = nullptr;
    _data = element;
}


template<class T, class Container, class Compare>
int binomial_tree<T, Container, Compare>::size() const
{
    return pow(2, _childreen.size());
}

template<class T, class Container, class Compare>
int binomial_tree<T, Container, Compare>::degree() const
{
    return _childreen.size();
}

template<class T, class Container, class Compare>
T& binomial_tree<T, Container, Compare>::top()
{
    return _data;
}

template<class T, class Container, class Compare>
std::pair<bool, typename binomial_tree<T, Container, Compare>::b_t_ptr>
        binomial_tree<T, Container, Compare>::merge(b_t_ptr first, b_t_ptr second)
{
    if(first->degree() != second->degree())
        return {false, first};

    Compare comp;
    if(!comp(first->_data, second->_data))
    {
        first->_parent = second;
        second->_childreen.push_front(first);
        return std::make_pair(true, second);
    }
    else
    {
        second->_parent = first;
        first->_childreen.push_front(second);
        return std::make_pair(true, first);
    }

    return std::make_pair(false, first);
}

//**********************************************************************
//binomial_heap
//**********************************************************************

template<class T, class Container, class Compare>
bool binomial_heap<T, Container, Compare>::empty() const
{
    return _list_of_roots.empty();
}

template<class T, class Container, class Compare>
int binomial_heap<T, Container, Compare>::size() const
{
    return _list_of_roots.size();
}

template<class T, class Container, class Compare>
int binomial_heap<T, Container, Compare>::true_size() const
{
    int size = 0;
    for(auto it = _list_of_roots.begin(), end = _list_of_roots.end();
        it != end; ++it)

        size += (*it)->size();
}

template<class T, class Container, class Compare>
typename binomial_heap<T, Container, Compare>::value_type
         binomial_heap<T, Container, Compare>::top()
{
    if(!this->empty())
        return (*_min_it)->top();
}

template<class T, class Container, class Compare>
void binomial_heap<T, Container, Compare>::push(const value_type& new_elem)
{
    std::shared_ptr<b_t> tree_ptr(new b_t(new_elem));
    this->fast_merge(tree_ptr);
}

template<class T, class Container, class Compare>
void binomial_heap<T, Container, Compare>::pop()
{
    if(this->empty())
        return;

    std::shared_ptr<b_t> ptr = *_min_it;

    _list_of_roots.erase(_min_it);

    for(auto it = ptr->_childreen.begin(), end = ptr ->_childreen.end();
        it != end; ++it)
    {
        this->merge(*(*it));
    }

    this->refresh_min();
}

template<class T, class Container, class Compare>
void binomial_heap<T, Container, Compare>::merge(b_t other)
{
    std::shared_ptr<b_t> other_ptr(new b_t(other));
    this->fast_merge(other_ptr);
}

template<class T, class Container, class Compare>
binomial_heap<T, Container, Compare>
binomial_heap<T, Container, Compare>::merge(binomial_heap<T, Container, Compare> first,
                                            binomial_heap<T, Container, Compare> second)
{
    return binomial_heap::fast_merge(first,second);
}

template<class T, class Container, class Compare>
bool binomial_heap<T, Container, Compare>::decrease_key(typename b_t::b_t_ptr node, value_type key)
{
    Compare comp;
    if(comp(key, node->_data))
        return true;

    node->_data = key;
    while(node->_parent != nullptr && comp(key, node->_parent->_data))
    {
        auto it = std::find(node->_parent->_childreen.begin(),
                            node->_parent->_childreen.end(), node);
        *it = node->_parent;
        std::swap(node->_childreen, node->_parent->_childreen);
        typename b_t::b_t_ptr buf_parent = node->_parent;
        node->_parent = node->_parent->_parent;
        node->_parent->_parent = buf_parent;
    }

    return true;
}

template<class T, class Container, class Compare>
bool binomial_heap<T, Container, Compare>::delete_key(typename b_t::b_t_ptr node, value_type key)
{
    bool erase_tree = false;
    while(node->_parent != nullptr)
    {
        if(node->_parent->_parent == nullptr)
        {
            _list_of_roots.erase
            (
                std::find(_list_of_roots.begin(),
                          _list_of_roots.end(),
                          node->_parent)
            );
            erase_tree = true;
        }

        auto it = std::find(node->_parent->_childreen.begin(),
                            node->_parent->_childreen.end(), node);
        *it = node->_parent;
        std::swap(node->_childreen, node->_parent->_childreen);
        typename b_t::b_t_ptr buf_parent = node->_parent;
        node->_parent = node->_parent->_parent;
        node->_parent->_parent = buf_parent;
    }

    if(!erase_tree)
    {
        //if node == first root of tree
        _list_of_roots.erase
        (
            std::find(_list_of_roots.begin(),
                      _list_of_roots.end(),
                      node)
        );
    }

    //this tree erase from heap
    //now root == node and we will add all root's childreen in heap
    for(auto it = node->_childreen.begin(), end = node->_childreen.end();
        it != end; ++it)
    {
        this->fast_merge(*it);
    }
}

template<class T, class Container, class Compare>
void binomial_heap<T, Container, Compare>::refresh_min()
{
    _min_it = _list_of_roots.begin();
    Compare comp;

    for(auto it = _list_of_roots.begin(), end = _list_of_roots.end();
        it != end; ++it)
    {
        if(comp((*it)->top(), (*_min_it)->top()) )
            _min_it = it;
    }
}

template<class T, class Container, class Compare>
void binomial_heap<T, Container, Compare>::fast_merge(std::shared_ptr<b_t> tree_ptr)
{
    Compare comp;
    bool tree_is_min = _list_of_roots.empty() ||
                        comp(tree_ptr->_data, (*_min_it)->_data);
    bool flag = true;

    auto it = _list_of_roots.begin(), end = _list_of_roots.end();
    while(flag)
    {
        while(it != end && (*it)->degree() < tree_ptr->degree())
            ++it;

        if(it == end || (*it)->degree() > tree_ptr->degree())
        {
            it = _list_of_roots.insert(it, tree_ptr);
            flag = false;
        }
        else if((*it)->degree() == tree_ptr->degree())
        {
            if(it == _min_it)
                tree_is_min = true;

            tree_ptr = (b_t::merge(*it, tree_ptr)).second;
            auto del = it;
            ++it;
            _list_of_roots.erase(del);
        }

    }

    if(tree_is_min)
    {
        _min_it = it;
    }
}

template<class T, class Container, class Compare>
binomial_heap<T, Container, Compare>&
binomial_heap<T, Container, Compare>::fast_merge(binomial_heap& first,
                                                 binomial_heap& second)
{
    if(first._list_of_roots.size() < second._list_of_roots.size())
    {
        for(auto it = first._list_of_roots.begin(), end = first._list_of_roots.end();
            it != end; ++it)
        {
            second.fast_merge(*it);
        }

        return second;
    }
    else
    {
        for(auto it = second._list_of_roots.begin(), end = second._list_of_roots.end();
            it != end; ++it)
        {
            first.fast_merge(*it);
        }

        return first;
    }
}
