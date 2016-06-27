#include <array>
#include <atomic>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "barrier.h"

void thread_func(int number_thread, barrier& bar)
{
    for(int i = 0; i < 5; ++i)
    {
        //bar.enter(number_thread);
        bar.enter();
    }
}

int main()
{
    int a = 3;
    barrier bar(a);

    std::thread t1(thread_func, 0, std::ref(bar));
    std::thread t2(thread_func, 1, std::ref(bar));
    std::thread t3(thread_func, 2, std::ref(bar));
    /*
    std::thread t4(thread_func, 3, std::ref(bar));
    std::thread t5(thread_func, 4, std::ref(bar));
    std::thread t6(thread_func, 5, std::ref(bar));
    std::thread t7(thread_func, 6, std::ref(bar));
    std::thread t8(thread_func, 7, std::ref(bar));
*/
    /*
    std::thread t9(thread_func, 8, std::ref(mutex), std::ref(x));
    std::thread t10(thread_func, 9, std::ref(mutex), std::ref(x));
*/

    t1.join();

    t2.join();

    t3.join();
/*
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();
*/
    /*
    t9.join();
    t10.join();
*/
    return 0;
}
