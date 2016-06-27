#include <iostream>
#include "barrier.h"

//***********************************************************************
//barrier
//***********************************************************************
/*
class barrier
{
public:
    barrier(const int& num_threads);
    barrier(const barrier& other) = delete;

    void enter(int number);

private:
    int _num_threads;
    int _amount_wait;
    int _era;
    std::mutex _mtx;
    std::condition_variable _cv;
};

barrier::barrier(const int& num_threads):
        _num_threads(num_threads), _amount_wait(0), _era(0) {}

void barrier::enter(int number)
{
    std::unique_lock<std::mutex> lock(_mtx);
    int cur_era = _era;
    ++_amount_wait;

    if(_amount_wait >= _num_threads)
    {
        //std::cout << "(w: " << _amount_wait << ", n: " << _num_threads << ")" << std::endl;
        ++_era;
        _amount_wait -= _num_threads;
        _cv.notify_all();
    }

    while(_era == cur_era)
    {
        _cv.wait(lock);
    }

    //std::cout << "(" << cur_era << ", " << _era << ", " << number << ") ";
}
*/
