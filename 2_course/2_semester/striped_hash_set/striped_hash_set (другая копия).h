#pragma once
#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <exception>
#include <forward_list>
#include <functional>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <vector>


template <typename T, class H = std::hash<T> >
class striped_hash_set
{
public:
    striped_hash_set(size_t num_stripes, double load_factor = 4, double growth_factor = 1.4);
    striped_hash_set(const striped_hash_set& other) = delete;

    void add(const T& e);
    void remove(const T& e);
    bool contains(const T& e);

private:
    std::vector<std::forward_list<T> > _table;
    std::vector<std::shared_mutex> _v_mutex;
    H _hash_func;
    size_t _num_stripes;
    size_t _size_of_table;
    std::atomic<size_t> _amount_of_elem;
    double _load_factor;
    double _growth_factor;

    void grow(size_t cur_amount);
    int get_index_of_mutex(const T& e);
    int get_index_in_table(const T& e);
};

template <typename T, class H>
striped_hash_set<T, H>::striped_hash_set(size_t num_stripes, double load_factor, double growth_factor):
    _table(30/num_stripes*num_stripes), _v_mutex(num_stripes),
    _num_stripes(num_stripes), _size_of_table(30/num_stripes*num_stripes), _amount_of_elem(0),
    _load_factor(load_factor), _growth_factor(growth_factor){}

template <typename T, class H>
void striped_hash_set<T, H>::add(const T& e)
{
    int m_index = this->get_index_of_mutex(e),
        t_index = this->get_index_in_table(e);

    std::unique_lock<std::shared_mutex> lock(_v_mutex[m_index]);

    if(std::find(_table[t_index].begin(), _table[t_index].end(), e) == _table[t_index].end())
    {
        _table[t_index].push_front(e);
        ++_amount_of_elem;

        if(_amount_of_elem/_size_of_table >= _load_factor)
        {
            size_t cur_amount = _amount_of_elem;
            lock.unlock();

            grow(cur_amount);
            return;
        }
    }
}

template <typename T, class H>
void striped_hash_set<T, H>::remove(const T& e)
{
    int m_index = this->get_index_of_mutex(e),
        t_index = this->get_index_in_table(e);

    std::unique_lock<std::shared_mutex> lock(_v_mutex[m_index]);

    auto it = std::find(_table[t_index].begin(), _table[t_index].end(), e);
    if(it != _table[t_index].end())
    {
        _table[t_index].erase_after(it);
        --_amount_of_elem;
    }
}

template <typename T, class H>
bool striped_hash_set<T, H>::contains(const T &e)
{
    int m_index = this->get_index_of_mutex(e),
        t_index = this->get_index_in_table(e);

    std::shared_lock<std::shared_mutex> lock(_v_mutex[m_index]);

    bool check = std::find(_table[t_index].begin(), _table[t_index].end(), e) != _table[t_index].end();

    return check;
}

template <typename T, class H>
void striped_hash_set<T, H>::grow(size_t cur_amount)
{
    std::vector<unique_lock<std::shared_mutex> > mutexes;
    mutexes.emplace_back(_v_mutex[0]);

    if(cur_amount < _amount_of_elem)
    {
        lock.unlock();
        return;
    }

    for(auto it = ++_v_mutex.begin(), end = _v_mutex.end();
        it != end; ++it)
    {
        mutexes.emplace_back(*it);
    }

    std::vector<std::forward_list<T> > new_table(_size_of_table*_growth_factor/_num_stripes*_num_stripes);
    _size_of_table = _table.size();

    for(auto t_it = _table.begin(), t_end = _table.end();
        t_it != t_end; ++t_it)
    {
        for(auto it = t_it->begin(), end = t_it->end();
            it != end; ++it)
        {
            new_table[this->get_index_in_table(*it)].push_front(*it);
        }
    }

    _table = std::move(new_table);
}


template <typename T, class H>
int striped_hash_set<T, H>::get_index_of_mutex(const T &e)
{
    return (_hash_func(e) % _size_of_table) % _num_stripes;
}

template <typename T, class H>
int striped_hash_set<T, H>::get_index_in_table(const T &e)
{
    return (_hash_func(e) % _size_of_table);
}
