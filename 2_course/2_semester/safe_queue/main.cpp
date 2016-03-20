#include <iostream>
#include <math.h>
#include <mutex>
#include <thread>
#include "safe_queue.h"

template<typename T>
void producer_func(thread_safe_queue<T>& queue)
{
    for(int i = 0; i < 1000000; ++i)
    {
        queue.enqueue(1);
    }

    queue.shutdown();

    for(int i = 0; i < 1000; ++i)
    {
        queue.enqueue(1);
    }
}

template<typename T>
void consumer_func(std::mutex &mutex, int &x, thread_safe_queue<T>& queue)
{
    int summand;

    while(queue.pop(summand))
    {
        std::unique_lock<std::mutex> lock(mutex);
        x += summand;
    }
}


int main()
{
    std::mutex mutex;
    thread_safe_queue<int> queue(10000);
    int x = 0;

    std::thread producer(producer_func<int>, std::ref(queue));

    std::thread t1(consumer_func<int>, std::ref(mutex), std::ref(x), std::ref(queue));


    std::thread t2(consumer_func<int>, std::ref(mutex), std::ref(x), std::ref(queue));

    std::thread t3(consumer_func<int>, std::ref(mutex), std::ref(x), std::ref(queue));

/*
    std::thread t4(consumer_func<int>, std::ref(mutex), std::ref(x), std::ref(queue));
    std::thread t5(consumer_func<int>, std::ref(mutex), std::ref(x), std::ref(queue));
    */
    /*
    std::thread t6(thread_func, 5, std::ref(mutex), std::ref(x));
    std::thread t7(thread_func, 6, std::ref(mutex), std::ref(x));
    std::thread t8(thread_func, 7, std::ref(mutex), std::ref(x));
    std::thread t9(thread_func, 8, std::ref(mutex), std::ref(x));
    std::thread t10(thread_func, 9, std::ref(mutex), std::ref(x));
*/
    producer.join();
    t1.join();

    t2.join();

    t3.join();

    /*
    t4.join();

    t5.join();
    */
    /*
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
*/
    std::cout << x << std::endl;
    return 0;
}
