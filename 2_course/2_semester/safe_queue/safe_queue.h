#pragma once
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>


template<typename T>
class thread_safe_queue
{
public:
    thread_safe_queue(int capacity);
    thread_safe_queue(const thread_safe_queue& other) = delete;

    void enqueue(const T& item);
    void enqueue(T&& item);
    bool pop(T& item);

    void shutdown();

private:
    std::atomic<int> _capacity;     //не уверен что нужен atomic
    std::queue<T> _queue;
    std::mutex _mtx;
    std::condition_variable _cv_empty;
    std::condition_variable _cv_full;
    std::atomic<bool> _shutdown;
};


template<typename T>
thread_safe_queue<T>::thread_safe_queue(int capacity):
                            _capacity(capacity), _shutdown(false) {}

template<typename T>
void thread_safe_queue<T>::enqueue(const T& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.size() >= _capacity && !_shutdown.load())
    {
        _cv_full.wait(lock);
    }

    if(!_shutdown.load())
    {
        _queue.push(item);
        _cv_empty.notify_one();
    }
}

template<typename T>
void thread_safe_queue<T>::enqueue(T&& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.size() >= _capacity && !_shutdown.load())
    {
        _cv_full.wait(lock);
    }

    if(!_shutdown.load())
    {
        _queue.push(std::move(item));
        //_queue.push(std::move(item));
        _cv_empty.notify_one();
    }
}


template<typename T>
bool thread_safe_queue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(_mtx);

    while(_queue.empty() && !_shutdown.load())
    {
        _cv_empty.wait(lock);
    }

    if(_shutdown.load() && _queue.empty())
        return false;

    item = std::move(_queue.front());
    _queue.pop();
    _cv_full.notify_one();
    return true;
}

template<typename T>
void thread_safe_queue<T>::shutdown()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _shutdown.store(true);
    _cv_empty.notify_all();
}
