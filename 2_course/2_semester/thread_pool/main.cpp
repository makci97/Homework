#include <atomic>
#include <iostream>
#include <math.h>
#include <thread>
#include "thread_pool.h"

/*
Простейший тест: 
в thread_pool отправляется функция,
которая атомарно инкрементирует счетчик,
после чего он сравнивается с правильным ответом.
*/

std::atomic<int> Result(0);

int func(std::atomic<int>& x)
{
    ++Result;
    ++x;
    return x;
}

int main()
{
    int num = 100000;
    std::atomic<int> x(0);
    thread_pool<int> pool(3);

    for(int i = 0; i < num; ++i)
    {
        auto future = pool.submit(std::bind(func, std::ref(x)));

        if(i % (num/10) == 0)
            std::cout << "x = " << x << "\t res = " << Result
                      << "\t future = " << future.get() << std::endl;
    }

    std::cout << "LAST:: x = " << x << "\t res = " << Result << std::endl;

    std::cout << (x == num) << std::endl;

    return 0;
}
