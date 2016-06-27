#pragma once
#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <exception>
#include <forward_list>
#include <functional>
#include <math.h>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class semaphore
{
public:
    semaphore(size_t c): count(c) {}
    semaphore(const semaphore& other) = delete;

    void wait();
    void signal();

private:
    std::mutex mtx;
    std::condition_variable signal_cv;
    std::atomic<size_t> count;
};

void semaphore::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    while(count == 0)
    {
        signal_cv.wait(lock);
    }
    count -= 1;
}

void semaphore::signal(){
    std::lock_guard<std::mutex> lock(mtx);
    count += 1;
    signal_cv.notify_one();
}

class rw_mutex
{
public:
    rw_mutex(): ring(1), readers(0) {}
    rw_mutex(const rw_mutex& other) = delete;

    void write_lock();
    void write_unlock();

    void read_lock();
    void read_unlock();

private:
    semaphore ring;
    std::mutex turnstile;
    std::mutex lightswitch;
    std::atomic<int> readers;
};

void rw_mutex::write_lock()
{
    turnstile.lock();
    ring.wait();
    turnstile.unlock();
}

void rw_mutex::write_unlock()
{
    ring.signal();
}

void rw_mutex::read_lock()
{
    turnstile.lock();
    turnstile.unlock();
    lightswitch.lock();
    ++readers;

    if (readers == 1)
        ring.wait();

    lightswitch.unlock();
}

void rw_mutex::read_unlock()
{
    lightswitch.lock();
    --readers;

    if(readers == 0)
        ring.signal();

    lightswitch.unlock();
}


template <typename T, class H = std::hash<T> >
class striped_hash_set
{
public:
    explicit striped_hash_set(size_t num_stripes, size_t first_size = 30, size_t load_factor = 4, size_t growth_factor = 4);
    striped_hash_set(const striped_hash_set& other) = delete;

    void add(const T& e);
    void remove(const T& e);
    bool contains(const T& e);

private:
    std::vector<std::forward_list<T> > _table;
    std::vector<rw_mutex> _v_mutex;
    H _hash_func;
    size_t _num_stripes;
    size_t _size_of_table;
    std::atomic<size_t> _amount_of_elem;
    size_t _load_factor;
    size_t _growth_factor;

    void grow(size_t cur_size);
    int get_index_of_mutex(const T& e);
    int get_index_in_table(const T& e);
};

template <typename T, class H>
striped_hash_set<T, H>::striped_hash_set(size_t num_stripes, size_t first_size, size_t load_factor, size_t growth_factor):
    _table(ceil(double(first_size)/num_stripes)*num_stripes), _v_mutex(num_stripes),
    _num_stripes(num_stripes), _size_of_table(ceil(double(first_size)/num_stripes)*num_stripes),
    _amount_of_elem(0), _load_factor(load_factor), _growth_factor(growth_factor){}

template <typename T, class H>
void striped_hash_set<T, H>::add(const T& e)
{
    int m_index = this->get_index_of_mutex(e);

    _v_mutex[m_index].write_lock();
    int t_index = this->get_index_in_table(e);

    if(std::find(_table[t_index].begin(), _table[t_index].end(), e) == _table[t_index].end())
    {
        _table[t_index].push_front(e);
        ++_amount_of_elem;

        if(_amount_of_elem/_size_of_table >= _load_factor)
        {
            size_t cur_size = _size_of_table;
            _v_mutex[m_index].write_unlock();

            grow(cur_size);
            return;
        }
    }

    _v_mutex[m_index].write_unlock();
}

template <typename T, class H>
void striped_hash_set<T, H>::remove(const T& e)
{
    int m_index = this->get_index_of_mutex(e);

    _v_mutex[m_index].write_lock();

    int t_index = this->get_index_in_table(e);

    auto it = std::find(_table[t_index].begin(), _table[t_index].end(), e);
    if(it != _table[t_index].end())
    {
        _table[t_index].remove(*it);
        --_amount_of_elem;
    }

    _v_mutex[m_index].write_unlock();
}

template <typename T, class H>
bool striped_hash_set<T, H>::contains(const T &e)
{
    int m_index = this->get_index_of_mutex(e);

    _v_mutex[m_index].read_lock();

    int t_index = this->get_index_in_table(e);

    bool check = std::find(_table[t_index].begin(), _table[t_index].end(), e) != _table[t_index].end();

    _v_mutex[m_index].read_unlock();

    return check;
}

template <typename T, class H>
void striped_hash_set<T, H>::grow(size_t cur_size)
{
    _v_mutex[0].write_lock();
    if(cur_size < _size_of_table)
    {
        _v_mutex[0].write_unlock();
        return;
    }

    for(auto it = ++_v_mutex.begin(), end = _v_mutex.end();
        it != end; ++it)
    {
        it->write_lock();
    }

    std::vector<std::forward_list<T> > new_table(ceil(double(_size_of_table*_growth_factor)/_num_stripes)*_num_stripes);
    _size_of_table = new_table.size();
    assert(_size_of_table % _num_stripes == 0);

    for(auto& t_it : _table)
    {
        for(auto& it : t_it)
        {
            new_table[this->get_index_in_table(it)].push_front(it);
        }
    }

    _table = std::move(new_table);

    for(auto it = _v_mutex.rbegin(), end = _v_mutex.rend();
        it != end; ++it)
    {
        it->write_unlock();
    }
}


template <typename T, class H>
int striped_hash_set<T, H>::get_index_of_mutex(const T &e)
{
    return _hash_func(e) % _num_stripes;
}

template <typename T, class H>
int striped_hash_set<T, H>::get_index_in_table(const T &e)
{
    return (_hash_func(e) % _size_of_table);
}
