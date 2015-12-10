#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

std::random_device Rand;

template<typename Key,
         class CompareKey = std::less<Key>
        >
struct node_treap
{
    node_treap():   _count(0), _l(NULL), _r(NULL), _p(NULL){}
    node_treap(Key key): _key(key), _prior(Rand()),
                        _count(0), _l(NULL), _r(NULL), _p(NULL){}

    Key _key;
    int _prior;
    int _count;     //amount of all node's childreen
    node_treap *_l;
    node_treap *_r;
    node_treap *_p;
};

//root's priority is maximal
template<typename Key,
         class CompareKey = std::less<Key>
        >
class treap
{
public:
    treap():_root(NULL) {}
    treap(const std::vector<Key>& base);
    ~treap();

    std::pair<treap, treap> split(const Key& key);
    void merge(treap &other);        //all keys this < all keys other
    void insert(Key key);
    void remove(Key key);
    bool empty() { return _root == NULL;}

    class iterator
    {
    public:
        iterator(): _cur(NULL) {}
        iterator(treap<Key, CompareKey>& tree):_cur(tree._root), _tree(&tree) {}

        inline void erase()
        {
            _tree->remove(_cur->_key);
            _cur = _tree->_root;
        }

        inline Key get_key() { return (_cur == NULL) ? :_cur->_key;}
        inline int get_count() { return (_cur == NULL) ? -1:_cur->_count;}
        inline int get_count_l()
        {
            return (_cur == NULL || _cur->_l == NULL)? -1:_cur->_l->_count;
        }

        inline int get_count_r()
        {
            return (_cur == NULL || _cur->_r == NULL)? -1:_cur->_r->_count;
        }

        inline void go_to_left()
        {
            if(_cur != NULL && _cur->_l != NULL) _cur = _cur->_l;
        }
        inline void go_to_right()
        {
            if(_cur != NULL && _cur->_r != NULL) _cur = _cur->_r;
        }

    private:
        node_treap<Key, CompareKey>* _cur;
        treap<Key, CompareKey>* _tree;
    };

private:
    typedef node_treap<Key, CompareKey>* reference;

    reference _root;

    inline static void update_count(reference node);
    inline void update_all_count_down(reference node);
    inline void update_all_count_up(reference node);
    inline void delete_node(reference node);
    void helper_for_split(reference tree, const Key& key,
                          reference& first, reference&second);
    static void helper_for_merge(reference &result,
                          reference first, reference second);
};

//from min key to max
template<typename Key, class CompareKey>
treap<Key, CompareKey>::treap(const std::vector<Key> &base)
{
    reference cur;
    auto it = base.begin(), end = base.end();

    if(it != end)
        _root = new node_treap<Key, CompareKey>(*(it++));
    else
        _root = new node_treap<Key, CompareKey>();

    cur = _root;

    for(; it != end; ++it)
    {
        reference new_node =
                new node_treap<Key, CompareKey>(*it);

        new_node->_prior = Rand();
        if(cur->_prior > new_node->_prior)
        {
            cur->_r = new_node;
            new_node->_p = cur;
        }
        else
        {
            while(new_node->_prior > cur->_prior && cur->_p != NULL)
                cur = cur->_p;

            if(cur->_p == NULL && new_node->_prior > cur->_prior)
            {
                //cur == root
                _root->_p = new_node;
                new_node->_l =_root;
                _root = new_node;
            }
            else
            {
                //cur != root && new_node->_prior <= cur->_prior
                cur->_r->_p = new_node;
                new_node->_l = cur->_r;

                new_node->_p = cur;
                cur->_r = new_node;
            }
        }

        cur = new_node;
    }

    this->update_all_count_down(_root);
}


template<typename Key, class CompareKey>
treap<Key, CompareKey>::~treap()
{
    if(_root != NULL)
        delete_node(_root);
}

//in first all keys <= key < all keys in second
template<typename Key, class CompareKey>
std::pair<treap<Key, CompareKey>, treap<Key, CompareKey> >
    treap<Key, CompareKey>::split(const Key& key)
{
    treap<Key, CompareKey> tree = *this;
    treap<Key, CompareKey> first;
    treap<Key, CompareKey> second;

    this->helper_for_split(tree._root, key, first._root, second._root);
    first._root->_p = NULL;
    second._root->_p = NULL;
    return std::make_pair(first, second);
}

template<typename Key, class CompareKey>
void treap<Key, CompareKey>::helper_for_split(reference tree, const Key& key,
                                              reference& first, reference& second)
{
    CompareKey comp;
    if(tree == NULL)
    {
        first = NULL;
        second = NULL;
    }
    else if(comp(tree->_key, key) )
    {
        this->helper_for_split(tree->_r, key, tree->_r, second);
        first = tree;
    }
    else
    {
        this->helper_for_split(tree->_l, key, first, tree->_l);
        second = tree;
    }

    treap<Key, CompareKey>::update_count(first);
    treap<Key, CompareKey>::update_count(second);
    treap<Key, CompareKey>::update_count(tree);

    return;
}

//in first all keys < in second all keys
template<typename Key, class CompareKey>
void treap<Key, CompareKey>::merge(treap& other)
{
    treap<Key, CompareKey>::helper_for_merge(_root, _root, other._root);
    other._root = NULL;
}

template<typename Key, class CompareKey>
void treap<Key, CompareKey>::helper_for_merge(reference& result,
                      reference first, reference second)
{
    if(first == NULL || second == NULL)
        result = (first != NULL) ? first : second;
    else if(first->_prior > second->_prior)
    {
        result = first;
        reference buf;
        treap<Key, CompareKey>::helper_for_merge(buf, first->_r, second);

        if(first->_l != NULL)
            first->_l->_p = result;
        result->_l = first->_l;

        if(buf != NULL)
            buf->_p = result;
        result->_r = buf;
    }
    else
    {
        result = second;
        reference buf;
        treap<Key, CompareKey>::helper_for_merge(buf, first, second->_l);

        if(buf != NULL)
            buf->_p = result;
        result->_l = buf;

        if(second->_r != NULL)
            second->_r->_p = result;
        result->_r = second->_r;
    }

    treap<Key, CompareKey>::update_count(first);
    treap<Key, CompareKey>::update_count(second);
    treap<Key, CompareKey>::update_count(result);
}

template<typename Key, class CompareKey>
void treap<Key, CompareKey>::insert(Key key)
{
    CompareKey key_comp;
    reference new_node =
            new node_treap<Key, CompareKey>(key);

    new_node->_prior = Rand();

    if(_root == NULL)
    {
        //tree is empty
        _root = new_node;
        return;
    }
    else if(new_node->_prior > _root->_prior)
    {
        //node -> new root
        _root->_p = new_node;

        if(key_comp(key, _root->_key))
        {
            //root's key > node's key
            new_node->_r = _root;

        }
        else
        {
            //root's key <= node's key
            new_node->_l = _root;
        }

        treap<Key, CompareKey>::update_count(new_node);
        _root = new_node;
        return;
    }

    //node will be inside tree (node !-> new root)
    reference cur = _root;
    while(new_node->_prior <= cur->_prior)
    {
        if(key_comp(key, cur->_key))
        {
            if(cur->_l == NULL)
            {
                //node is left list for cur
                new_node->_p = cur;
                cur->_l = new_node;
                treap<Key, CompareKey>::update_all_count_up(new_node);
                return;
            }

            cur = cur->_l;
        }
        else if(key_comp(cur->_key, key))
        {
            if(cur->_r == NULL)
            {
                //node is right list for cur
                new_node->_p = cur;
                cur->_r = new_node;
                treap<Key, CompareKey>::update_all_count_up(new_node);
                return;
            }

            cur = cur->_r;
        }
        else
        {
            //node's key == cur's key
            break;
        }
    }


    //node's key == cur's key
    //or
    //node's prior > cur's prior
    reference first;
    reference second;
    reference parent_of_cur = cur->_p;

    //parent_of_cur != NULL
    bool cur_is_left = (cur == parent_of_cur->_l) ? 1 : 0;

    this->helper_for_split(cur, key, first, second);

    new_node->_l = first;
    new_node->_r = second;
    new_node->_p = parent_of_cur;

    if(cur_is_left)
    {
        parent_of_cur->_l = new_node;
    }
    else
    {
        parent_of_cur->_r = new_node;
    }

    treap<Key, CompareKey>::update_all_count_up(new_node);
}

template<typename Key, class CompareKey>
void treap<Key, CompareKey>::remove(Key key)
{
    CompareKey key_comp;

    if(_root == NULL)
    {
        //tree is empty
        return;
    }

    reference cur = _root;
    while(true)
    {
        if(key_comp(key, cur->_key))
        {
            if(cur->_l == NULL)
            {
                //node with this key not found
                return;
            }

            cur = cur->_l;
        }
        else if(key_comp(cur->_key, key))
        {
            if(cur->_r == NULL)
            {
                //node with this key not found
                return;
            }

            cur = cur->_r;
        }
        else
        {
            //key == cur's key
            break;
        }
    }
    //cur = removable node != NULL

    reference result;
    reference parent = cur->_p;
    treap<Key, CompareKey>::helper_for_merge(result, cur->_l, cur->_r);

    if(parent == NULL)
    {
        _root = result;
        if(result != NULL)
            result->_p = NULL;
    }
    else if(parent->_l == cur)
    {
        parent->_l = result;
        if(result != NULL)
            result->_p = parent;
    }
    else
    {
        parent->_r = result;
        if(result != NULL)
            result->_p = parent;
    }

    delete cur;

    treap<Key, CompareKey>::update_all_count_up(parent);
}


template<typename Key, class CompareKey>
inline void treap<Key, CompareKey>::update_all_count_down(reference node)
{
    if(node == NULL)
        return;

    this->update_all_count_down(node->_l);
    this->update_all_count_down(node->_r);

    treap<Key, CompareKey>::update_count(node);
}

template<typename Key, class CompareKey>
inline void treap<Key, CompareKey>::update_all_count_up(reference node)
{
    if(node == NULL)
        return;
    treap<Key, CompareKey>::update_count(node);

    this->update_all_count_up(node->_p);
}

template<typename Key, class CompareKey>
inline void treap<Key, CompareKey>::update_count(reference node)
{
    if(node == NULL)
        return;

    node->_count = 0;

    if(node->_l != NULL)
        node->_count += node->_l->_count + 1;
    if(node->_r != NULL)
        node->_count += node->_r->_count + 1;
}

template<typename Key, class CompareKey>
inline void treap<Key, CompareKey>::delete_node(reference node)
{
    if(node == NULL)
        return;

    if(node->_l != NULL)
        delete_node(node->_l);

    if(node->_r != NULL)
        delete_node(node->_r);

    delete node;
}

//*************************************************************
//Josephus problem
//*************************************************************

int Josephus_problem(int n, int k)
{
    if(n < 1 || k <= 0)
        return -1;

    std::vector<int> base;

    for(int i = 1; i <= n; ++i)
    {
        base.push_back(i);
    }

    treap<int> tree(base);

    treap<int>::iterator cur(tree);

    while(cur.get_count() != 0 && n > 1)
    {
        //while _root.count != 0
        int remain = k % n;
        if(remain == 0)
            remain = n;

        while(true)
        {
            if(cur.get_count_l() + 2 < remain)
            {
                //go in right branch
                remain -= cur.get_count_l() + 2;

                cur.go_to_right();
            }
            else if(cur.get_count_l() + 2 > remain)
            {
                //go in left branch
                cur.go_to_left();
            }
            else
            {
                //remove cur element
                cur.erase();
                break;
            }
        }
        //remove k-th element
        //cur = new_root
        --n;
    }

    //in tree remain last element
    assert(n == 1);
    return cur.get_key();
}
