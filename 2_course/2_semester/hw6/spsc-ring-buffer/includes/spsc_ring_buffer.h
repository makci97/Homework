#pragma once
#include <atomic>
#include <vector>

const int CACHE_LINE_SIZE = 64;

template <class Value>
struct buf_node
{
    buf_node() {}
    buf_node(const Value& date): _date(date) {}

    Value _date;
    char _pad[CACHE_LINE_SIZE];

};

template <class Value>
class spsc_ring_buffer{
public:
    explicit spsc_ring_buffer(size_t size):
        _size(size + 1), _head(0), _buffer(_size), _tail(0) {}

    bool enqueue(const Value& v);
    bool dequeue(Value& v);

private:
    size_t _size;
    std::atomic<size_t> _head;
    std::vector<buf_node<Value> > _buffer;
    std::atomic<size_t> _tail;

    bool full(size_t cur_head, size_t cur_tail)
    {
        return ((cur_tail + 1) - cur_head) % _size == 0;
    }

    bool empty(size_t cur_head, size_t cur_tail)
    {
        return cur_head == cur_tail;
    }
};

template <class Value>
bool spsc_ring_buffer<Value>::enqueue(const Value& v)
{
    size_t cur_head = _head.load(std::memory_order_acquire);
    size_t cur_tail = _tail.load(std::memory_order_acquire);

    if(full(cur_head, cur_tail) )
        return false;

    buf_node<Value> elem(v);
    _buffer[cur_tail] = elem;

    cur_tail = (cur_tail + 1) % _size;

    _tail.store(cur_tail, std::memory_order_release);

    return true;
}

template <class Value>
bool spsc_ring_buffer<Value>::dequeue(Value& v)
{
    size_t cur_tail = _tail.load(std::memory_order_acquire);
    size_t cur_head = _head.load(std::memory_order_acquire);

    if(empty(cur_head, cur_tail) )
        return false;

    v = _buffer[cur_head]._date;

    cur_head = (cur_head + 1) % _size;

    _head.store(cur_head, std::memory_order_release);

    return true;
}
