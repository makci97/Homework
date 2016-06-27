#include <iostream>
#include <cstdlib>
#include <ctime>
#include <future>
#include <math.h>
#include <mutex>
#include <thread>
#include "striped_hash_set.h"

const int SIZE = 1000000;

void add_func(striped_hash_set<int>& hash_table, std::mutex& rw, int num)
{
    for(int i = (num-1)*SIZE; i < num * SIZE; ++i)
    {
        //rw.lock();
        hash_table.add(i);
        //rw.unlock();
    }

    for(int i = 0; i < 10; ++i)
    {
        int number = rand()%(num * SIZE);

        rw.lock();

        if(hash_table.contains(number))
            std::cout << "Check::" << number << " contains in the table" << std::endl;
        else
            std::cout << "Check::" << number << " not contains in the table" << std::endl;

        rw.unlock();
    }

    for(int i = 0; i < 10; ++i)
    {
        int number = rand()%(num * SIZE);

        hash_table.remove(number);

        rw.lock();

        if(hash_table.contains(number))
            std::cout << "Delete::" << number << " contains in the table" << std::endl;
        else
            std::cout << "Delete::" << number << " not contains in the table" << std::endl;

        rw.unlock();
    }

}

void check_func(striped_hash_set<int>& hash_table, std::mutex& rw, int num)
{
    for(int i = 0; i < 10; ++i)
    {
        int number = rand()%(num * SIZE);

        rw.lock();

        if(hash_table.contains(number))
            std::cout << "CCheck::" << number << " contains in the table" << std::endl;
        else
            std::cout << "CCheck::" << number << " not contains in the table" << std::endl;

        rw.unlock();
    }

    for(int i = 0; i < 10; ++i)
    {
        int number = rand()%(num * SIZE);

        hash_table.remove(number);

        rw.lock();

        if(hash_table.contains(number))
            std::cout << "DDelete::" << number << " contains in the table" << std::endl;
        else
            std::cout << "DDelete::" << number << " not contains in the table" << std::endl;

        rw.unlock();
    }
}

int main()
{
    srand( time( 0 ) );
    striped_hash_set<int> hash_table(41);
    std::mutex rw;

    std::future<void> t1 = std::async(std::launch::async, add_func, std::ref(hash_table), std::ref(rw), 1);
    std::future<void> t2 = std::async(std::launch::async, add_func, std::ref(hash_table), std::ref(rw), 1);
    std::future<void> t3 = std::async(std::launch::async, add_func, std::ref(hash_table), std::ref(rw), 1);
    std::future<void> t4 = std::async(std::launch::async, check_func, std::ref(hash_table), std::ref(rw), 1);

    t1.wait();
    t2.wait();
    t3.wait();
    t4.wait();
    /*
    std::thread t1(add_func, std::ref(hash_table), std::ref(rw), 1);

    std::thread t2(add_func, std::ref(hash_table), std::ref(rw), 1);

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
    t1.join();

    t2.join();

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
