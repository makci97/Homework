#pragma once
#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <deque>
#include <stdexcept>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

const int SIZE_QUEUE = 10000;

template<typename Value, typename Container = std::deque<Value> >
class thread_safe_queue
{
public:
    explicit thread_safe_queue(size_t capacity);
    thread_safe_queue(const thread_safe_queue& other) = delete;

    void enqueue(const Value& item);
    void enqueue(Value&& item);
    bool dequeue(Value& item);

    void shutdown();

private:
    size_t _capacity;
    Container _queue;
    std::mutex _mtx;
    std::condition_variable _cv_empty;
    std::condition_variable _cv_full;
    bool _shutdown;
};


template<typename Value>
class thread_pool
{
public:
    explicit thread_pool();
    explicit thread_pool(size_t num_workers);
    ~thread_pool();
    thread_pool(const thread_pool& other) = delete;

    std::future<Value> submit(std::function<Value()> func);
    void shutdown();
    int get_num_workers();

private:
    thread_safe_queue<std::packaged_task<Value()> > _queue;
    size_t _num_workers;
    std::vector<std::thread> _threads;
    std::atomic<bool> _shutdown;

    void thread_function();
    size_t generate_num_workers(size_t num_workers = 0);
    void initialize_threads();
};

//********************************************************************************
//thread_safe_queue
//********************************************************************************
template<typename Value, typename Container>
thread_safe_queue<Value, Container>::thread_safe_queue(size_t capacity): _capacity(capacity), _shutdown(false) {}

template<typename Value, typename Container>
void thread_safe_queue<Value, Container>::enqueue(const Value& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.size() >= _capacity && !_shutdown)
    {
        _cv_full.wait(lock);
    }

    if(!_shutdown)
    {
        _queue.push_back(item);
        _cv_empty.notify_one();
    }
    else
    {
        throw std::out_of_range("Can not add element\n");
    }
}

template<typename Value, typename Container>
void thread_safe_queue<Value, Container>::enqueue(Value&& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.size() >= _capacity && !_shutdown)
    {
        _cv_full.wait(lock);
    }

    if(!_shutdown)
    {
        _queue.push_back(std::move(item));
        _cv_empty.notify_one();
    }
    else
    {
        throw std::out_of_range("Can not add element\n");
    }
}


template<typename Value, typename Container>
bool thread_safe_queue<Value, Container>::dequeue(Value& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.empty() && !_shutdown)
    {
        _cv_empty.wait(lock);
    }

    if(_shutdown && _queue.empty())
    {
        return false;
    }

    item = std::move(_queue.front());
    _queue.pop_front();
    _cv_full.notify_one();

    return true;
}

template<typename Value, typename Container>
void thread_safe_queue<Value, Container>::shutdown()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _shutdown = true;
    _cv_empty.notify_all();
    _cv_full.notify_all();
}

//********************************************************************************
//thread_pool
//********************************************************************************

template<typename Value>
size_t thread_pool<Value>::generate_num_workers(size_t num_workers)
{
    if(num_workers == 0)
        return std::max((size_t)1, (size_t)std::thread::hardware_concurrency());
    else
        return num_workers;
}

template<typename Value>
void thread_pool<Value>::initialize_threads()
{
    for(size_t i = 0; i < _num_workers; ++i)
    {
        _threads.emplace_back(&thread_pool<Value>::thread_function, this);
    }
}

template<typename Value>
thread_pool<Value>::thread_pool(): _queue(SIZE_QUEUE), _shutdown(false)
{
    _num_workers = this->generate_num_workers();
    this->initialize_threads();
}

template<typename Value>
thread_pool<Value>::thread_pool(size_t num_workers): _queue(SIZE_QUEUE), _shutdown(false)
{
    _num_workers = (size_t)std::max((int)1, (int)num_workers);
    this->initialize_threads();
}

template<typename Value>
thread_pool<Value>::~thread_pool()
{
    this -> shutdown();
}

template<typename Value>
std::future<Value> thread_pool<Value>::submit(std::function<Value()> func)
{
    std::packaged_task<Value()> new_task(func);
    std::future<Value> result = new_task.get_future();
    _queue.enqueue(std::move(new_task));
    return result;
}

template<typename Value>
void thread_pool<Value>::shutdown()
{
    if(!_shutdown.exchange(true))
    {
        _queue.shutdown();
        for(auto &it : _threads)
        {
            it.join();
        }
    }
}

template<typename Value>
int thread_pool<Value>::get_num_workers()
{
    return _num_workers;
}

template<typename Value>
void thread_pool<Value>::thread_function()
{
    std::packaged_task<Value()> cur_task;
    while(_queue.dequeue(cur_task))
    {
            cur_task();
    }
}
