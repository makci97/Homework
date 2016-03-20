#pragma once
#include <array>
#include <atomic>
#include <cassert>
#include <math.h>
#include <vector>

enum childreen{left, right, none};

class tree_mutex;

class peterson_mutex
{
    friend class tree_mutex;

public:
    peterson_mutex();

    void lock(int t);
    void unlock(int t);

protected:
    childreen who_locked();

private:
    std::array<std::atomic<bool>, 2> _want;
    std::atomic<int> _victim;
    std::atomic<childreen> _who_locked;
};

class tree_mutex
{
public:
    tree_mutex(int num_threads);

    void lock(int thread_index);
    void unlock(int thread_index);

private:
    int _hight_vec;
    std::vector<peterson_mutex> _mutex;
};

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

    _who_locked.store(childreen(t));
}

void peterson_mutex::unlock(int t)
{
    _who_locked.store(none);
    _want[t].store(false);
}

childreen peterson_mutex::who_locked()
{
    return _who_locked.load();
}

//***********************************************************************
//tree_mutex
//***********************************************************************
tree_mutex::tree_mutex(int num_threads): _hight_vec( (ceil(log(num_threads)/log(2)) == 0)? 1: ceil(log(num_threads)/log(2)) ),
    _mutex(pow(2, _hight_vec) - 1) {}

void tree_mutex::lock(int thread_index)
{
    int index = thread_index + pow(2, _hight_vec) - 1;
    //index of thread in tree

    while(index > 0)
    {
        //if right child of index than 1
        //else 0
        bool who_want_lock = ( (index % 2) == 0);

        //go up in tree
        index = floor((index - 1)/2);

        //call lock for mutex
        _mutex[index].lock(who_want_lock);

        //lock next mutex
    }
}

void tree_mutex::unlock(int thread_index)
{
    int index = 0;
    int size = _mutex.size();

    while(index < size)
    {
        int who_locked = _mutex[index].who_locked();
        _mutex[index].unlock(who_locked);
        index = 2*index + 1 + who_locked;
    }

    assert(thread_index + pow(2, _hight_vec) - 1 == index);
}


//***********************************************************************
//tricky_mutex
//***********************************************************************
class tricky_mutex {
public:
   tricky_mutex() : thread_count(0)
   {}

   void lock() {
       while (thread_count.fetch_add(1) > 0) {
           thread_count.fetch_sub(1);
       }
   }

   void unlock() {
       thread_count.fetch_sub(1);
   }

private:
   std::atomic<int> thread_count;
};

//***********************************************************************
//tas_spinlock
//***********************************************************************
class tas_spinlock {
public:
    tas_spinlock()
        : locked(false)
    {}

    void lock() {
        while (locked.exchange(true)) {
            // wait
        }
    }

    void unlock() {
        locked.store(false);
    }

private:
    std::atomic<bool> locked;
};

//***********************************************************************
//ticket_spinlock
//***********************************************************************
class ticket_spinlock {
private:
    using ticket = std::size_t;
public:
    ticket_spinlock()
        : owner_ticket(0)
        , next_free_ticket(0)
    {}

    void lock() {
        ticket this_thread_ticket = next_free_ticket.fetch_add(1);
        while (owner_ticket.load() != this_thread_ticket) {
            // wait
        }
    }

    void unlock() {
        owner_ticket.store(owner_ticket.load() + 1);
    }
private:
    std::atomic<int> owner_ticket;
    std::atomic<int> next_free_ticket;
};
