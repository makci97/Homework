#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

template<typename Key, typename Prior,
         class CompareKey = std::less<Key>,
         class ComparePrior = std::less<Prior>
        >
struct node_treap
{
    node_treap():   _count(0), _l(NULL), _r(NULL), _p(NULL){}
    node_treap(Key key, Prior prior):   _key(key), _prior(prior), _count(0),
                                        _l(NULL), _r(NULL), _p(NULL){}

    Key _key;
    Prior _prior;
    int _count;
    node_treap *_l;
    node_treap *_r;
    node_treap *_p;
};

template<typename Key, typename Prior,
         class CompareKey = std::less<Key>,
         class ComparePrior = std::less<Prior>
        >
class treap
{
public:
    treap(const std::vector<std::pair<Key, Prior> >& base);
    ~treap();

    std::pair<treap, treap> split(int key);
    static void merge(treap first, treap second, treap &result);    //all keys first < all keys second
    void insert(Key key, Prior prior);
    void remove(Key key);

private:
    node_treap<Key, Prior, CompareKey, ComparePrior> *_root;

    void update_count(node_treap<Key, Prior, CompareKey, ComparePrior> *node);
    inline void delete_node(node_treap<Key, Prior, CompareKey, ComparePrior> *node);
};


template<typename Key, typename Prior, class CompareKey, class ComparePrior>
treap<Key, Prior, CompareKey, ComparePrior>::treap(const std::vector<std::pair<Key, Prior> > &base)
{
    node_treap<Key, Prior, CompareKey, ComparePrior> *cur;
    auto it = base.begin(), end = base.end();

    if(it != end)
        _root = new node_treap<Key, Prior, CompareKey, ComparePrior>(it->first, it++ ->second);
    else
        _root = new node_treap<Key, Prior, CompareKey, ComparePrior>();

    cur = _root;

    for(; it != end; ++it)
    {
        node_treap<Key, Prior, CompareKey, ComparePrior>* new_node =
                new node_treap<Key, Prior, CompareKey, ComparePrior>(it->first, it->second);

        if(cur->_prior > it->second)
        {
            cur->_r = new_node;
        }
        else
        {
            while(it->second > cur->_prior && cur->_p != NULL)
                cur = cur->_p;

            if(cur->_p == NULL && it->second > cur->_prior)
            {
                new_node->_l =_root;
                _root = new_node;
            }
            else
            {
                new_node->_l = cur->_r;
                cur->_r = new_node;
            }
        }

        cur = new_node;
    }
}


template<typename Key, typename Prior, class CompareKey, class ComparePrior>
treap<Key, Prior, CompareKey, ComparePrior>::~treap()
{
    delete_node(_root);
}

template<typename Key, typename Prior, class CompareKey, class ComparePrior>
void treap<Key, Prior, CompareKey, ComparePrior>::merge(treap first, treap second, treap& result)
{
    if(first._root == NULL || second._root == NULL)
        result._root = first._root ? first._root != NULL : second._root;
    else if(first._root->_prior > second._root->_prior)
    {
        result._root = first._root;
        merge(*first._root->_r, second, second);
        result._root->_l = first._root;
        result._root->_r = second._root;
    }
    else
    {
        result._root = second._root;
        merge(first, second._root->_l, first);
        result._root->_l = first._root;
        result._root->_r = second._root;
    }
    return result;
}

template<typename Key, typename Prior, class CompareKey, class ComparePrior>
void treap<Key, Prior, CompareKey, ComparePrior>::insert(Key key, Prior prior)
{
    CompareKey key_comp;
    ComparePrior prior_comp;
    node_treap<Key, Prior, CompareKey, ComparePrior> *cur = _root;

    while(cur != NULL && key_comp(prior, cur->_prior))
    {
        if
    }
}

template<typename Key, typename Prior, class CompareKey, class ComparePrior>
void treap<Key, Prior, CompareKey, ComparePrior>::remove(Key key)
{

}

template<typename Key, typename Prior, class CompareKey, class ComparePrior>
void treap<Key, Prior, CompareKey, ComparePrior>::update_count(node_treap<Key, Prior, CompareKey, ComparePrior> *node)
{

}

template<typename Key, typename Prior, class CompareKey, class ComparePrior>
inline void treap<Key, Prior, CompareKey, ComparePrior>::delete_node(node_treap<Key, Prior, CompareKey, ComparePrior> *node)
{
    if(node->_l != NULL)
        delete_node(node->_l);

    if(node->_r != NULL)
        delete_node(node->_r);

    delete node;
}
