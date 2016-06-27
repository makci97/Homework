#pragma once
#include <mutex>
#include <condition_variable>

class barrier
{
public:
    explicit barrier(size_t num_threads);
    barrier(const barrier& other) = delete;

    void enter();

private:
    size_t _num_threads;
    size_t _amount_wait;
    size_t _era;
    std::mutex _mtx;
    std::condition_variable _cv;
};

barrier::barrier(size_t num_threads):
        _num_threads(num_threads), _amount_wait(0), _era(0) {}

void barrier::enter()
{
    std::unique_lock<std::mutex> lock(_mtx);
    size_t cur_era = _era;
    ++_amount_wait;

    if(_amount_wait >= _num_threads)
    {
        ++_era;
        _amount_wait -= _num_threads;
        _cv.notify_all();
    }

    while(_era == cur_era)
    {
        _cv.wait(lock);
    }
}
