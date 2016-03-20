#include <array>
#include <atomic>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include "tree_mutex.h"

/*
class peterson_mutex
{
public:
    peterson_mutex();

    void lock(int t);
    void unlock(int t);

private:
    std::array<std::atomic<bool>, 2> _want;
    std::atomic<int> _victim;
};

class tree_mutex
{
public:
    tree_mutex(int num_threads);

    void lock(int thread_index, bool _index_is_true = false);
    void unlock(int thread_index, bool _index_is_true = false);

private:
    int _hight_vec;
    std::vector<peterson_mutex> _mutex;
};

//-----------------------------------------------------------------------
//peterson_mutex
//-----------------------------------------------------------------------

peterson_mutex::peterson_mutex()
{
    _want[0].store(false);
    _want[1].store(false);
    _victim.store(0);
}

void peterson_mutex::lock(int t)
{
    _want[t].store(true);
    _victim.store(t);
    while (_want[1 - t].load() && _victim.load() == t)
    {
        // wait
        std::this_thread::yield();
    }
}

void peterson_mutex::unlock(int t)
{
    _want[t].store(false);
}

//-----------------------------------------------------------------------
//tree_mutex
//-----------------------------------------------------------------------
tree_mutex::tree_mutex(int num_threads): _hight_vec(ceil(log(num_threads)/log(2)) ), _mutex(pow(2, _hight_vec) - 1) {}

void tree_mutex::lock(int thread_index, bool _index_is_true)
{
    //when user call this function
    if(!_index_is_true)
    {
        thread_index += pow(2, _hight_vec) - 1;
    }

    //now thread_index is index of mutex/thread in tree
    //go up in tree
    int index = floor((thread_index - 1)/2);

    //if thread_index is right child of index than 1
    //else 0
    bool who_want_lock = thread_index == 2*(index + 1);

    //call lock for mutex
    _mutex[index].lock(who_want_lock);

    //lock next mutex
    if(index > 0)
    {
        this->lock(index, true);
    }
}

void tree_mutex::unlock(int thread_index, bool _index_is_true)
{
    //when user call this function
    if(!_index_is_true)
    {
        thread_index += pow(2, _hight_vec) - 1;
    }

    //now thread_index is index of mutex/thread in tree
    //go up in tree
    int index = floor((thread_index - 1)/2);

    //if thread_index is right child of index than 1
    //else 0
    bool who_want_unlock = thread_index == 2*(index + 1);

    //call lock for mutex
    _mutex[index].unlock(who_want_unlock);

    //lock next mutex
    if(index > 0)
    {
        this->unlock(index, true);
    }
}
*/

void thread_func(int number_thread, ticket_spinlock &mutex, int &x)
{
    for(int i = 0; i < 1000000; ++i)
    {
        /*
        mutex.lock(number_thread);
        ++x;
        mutex.unlock(number_thread);
        */
        mutex.lock();
        ++x;
        mutex.unlock();
    }
}

int main()
{
    /*
    for(int i = 0; i < 20; ++i)
        std::cout << i << ' ' << ((ceil(log(i)/log(2)) == 0)? 1: (ceil(log(i)/log(2)) )) << ' ' << pow(2,(ceil(log(i)/log(2)) == 0)? 1: (ceil(log(i)/log(2)) )) - 1 << std::endl;
    */
    //tree_mutex mutex(10);
    ticket_spinlock mutex;
    int x = 0;

    std::thread t1(thread_func, 0, std::ref(mutex), std::ref(x));

    std::thread t2(thread_func, 1, std::ref(mutex), std::ref(x));

    std::thread t3(thread_func, 2, std::ref(mutex), std::ref(x));

    std::thread t4(thread_func, 3, std::ref(mutex), std::ref(x));
    std::thread t5(thread_func, 4, std::ref(mutex), std::ref(x));
    /*
    std::thread t6(thread_func, 5, std::ref(mutex), std::ref(x));
    std::thread t7(thread_func, 6, std::ref(mutex), std::ref(x));
    std::thread t8(thread_func, 7, std::ref(mutex), std::ref(x));
    std::thread t9(thread_func, 8, std::ref(mutex), std::ref(x));
    std::thread t10(thread_func, 9, std::ref(mutex), std::ref(x));
*/

    t1.join();

    t2.join();

    t3.join();
    t4.join();

    t5.join();
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
