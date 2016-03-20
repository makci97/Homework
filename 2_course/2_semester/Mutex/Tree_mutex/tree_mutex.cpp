#include<math.h>
#include<thread>
#include "Tree_mutex.h"

//***********************************************************************
//peterson_mutex
//***********************************************************************

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

//***********************************************************************
//tree_mutex
//***********************************************************************
tree_mutex::tree_mutex(int num_threads): _num_threads(num_threads),
{
    _hight_vec = std::ceil(std::log(num_threads)/std::log(2));
    _mutex.resize(std::pow(2, _hight_vec) - 1);
}

void tree_mutex::lock(int thread_index, bool _index_is_true = false)
{
    //when user call this function
    if(!_index_is_true)
    {
        thread_index += std::pow(2, _hight_vec) - 1;
    }

    //now thread_index is index of mutex/thread in tree
    //go up in tree
    int index = std::floor((thread_index - 1)/2);

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
        thread_index += std::pow(2, _hight_vec) - 1;
    }

    //now thread_index is index of mutex/thread in tree
    //go up in tree
    int index = std::floor((thread_index - 1)/2);

    //call unlock for mutex
    _mutex[index].unlock();

    //lock next mutex
    if(index > 0)
    {
        this->unlock(index, true);
    }
}

