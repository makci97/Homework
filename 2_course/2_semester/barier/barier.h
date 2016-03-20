#pragma once
#include <mutex>
#include <condition_variable>

class barier
{
public:
    barier(int num_threads);
    barier(const barier& other) = delete;

    void enter(int number);

private:
    int _num_threads;
    int _amount_wait;
    int _era;
    std::mutex _mtx;
    std::condition_variable _cv;
};
