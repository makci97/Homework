#include <iostream>
#include "barier.h"

//***********************************************************************
//barier
//***********************************************************************
barier::barier(int num_threads):
        _num_threads(num_threads), _amount_wait(0), _era(0) {}

void barier::enter(int number)
{
    std::unique_lock<std::mutex> lock(_mtx);
    int cur_era = _era;
    ++_amount_wait;

    if(_amount_wait >= _num_threads)
    {
        std::cout << std::endl;
        ++_era;
        _amount_wait -= _num_threads;
        _cv.notify_all();
    }

    while(_era == cur_era)
    {
        _cv.wait(lock);
    }

    --_amount_wait;
    std::cout << number << ' ';
}
