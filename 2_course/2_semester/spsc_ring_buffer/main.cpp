#include <iostream>
#include <cstdlib>
#include <ctime>
#include <future>
#include <math.h>
#include <mutex>
#include <thread>
#include "spsc_ring_buffer.h"

const int SIZE = 10000000;

uint64_t producer_func(spsc_ring_buffer<int>& buffer)
{
    uint64_t result = 0;

    for(int i = 0; i < SIZE; ++i)
    {
        while(!buffer.enqueue(i))
            std::this_thread::yield();

        result += i;
    }

    return result;
}

uint64_t consumed_func(spsc_ring_buffer<int>& buffer)
{
    uint64_t result = 0;
    int elem = 0;

    for(int i = 0; i < SIZE; ++i)
    {
        while(!buffer.dequeue(elem))
            std::this_thread::yield();

        result += elem;
    }

    return result;
}

int main()
{
    std::cout << ((120 - 137) % 1025 == 0) << std::endl;
    spsc_ring_buffer<int> buffer(1024);

    std::future<uint64_t> produced_sum = std::async(std::launch::async, producer_func, std::ref(buffer));
    std::future<uint64_t> consumed_sum = std::async(std::launch::async, consumed_func, std::ref(buffer));

    produced_sum.wait();
    consumed_sum.wait();

    std::cout << produced_sum.get() << ' ' << consumed_sum.get() << std::endl;

    /*
    std::thread t3(add_func, std::ref(hash_table), std::ref(rw), 1);

    std::thread t4(check_func, std::ref(hash_table), std::ref(rw), 1);

    /*
    std::thread t5(delete_func<int>, std::ref(hash_table), 1);
    */

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

    /*
    t3.join();


    t4.join();
    /*
    t5.join();
    */
    /*
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
*/
    return 0;
}
