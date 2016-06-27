#pragma once
#include <atomic>
#include <iostream>
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

    bool enqueue(Value v);
    bool dequeue(Value& v);

private:
    size_t _size;
    std::atomic<size_t> _head;
    std::vector<buf_node<Value> > _buffer;
    std::atomic<size_t> _tail;

    bool full(size_t cur_head, size_t cur_tail)
    {
        return ( ( (cur_tail + 1) % _size) == cur_head);
    }

    bool empty(size_t cur_head, size_t cur_tail)
    {
        return cur_head == cur_tail;
    }
};

template <class Value>
bool spsc_ring_buffer<Value>::enqueue(Value v)
{
    size_t cur_tail = _tail.load(std::memory_order_relaxed);
    size_t cur_head = _head.load(std::memory_order_acquire);

    if(full(cur_head, cur_tail) )
    {
        return false;
    }

    _buffer[cur_tail]._date = v;

    _tail.store((cur_tail + 1) % _size, std::memory_order_release);
    return true;
}

template <class Value>
bool spsc_ring_buffer<Value>::dequeue(Value& v)
{
    size_t cur_head = _head.load(std::memory_order_relaxed);
    size_t cur_tail = _tail.load(std::memory_order_acquire);

    if(empty(cur_head, cur_tail) )
        return false;

    v = _buffer[cur_head]._date;

    _head.store((cur_head + 1) % _size, std::memory_order_release);
    return true;
}

/*
 * Memory ordering именно такие, потому что нам необходимо гарантировать отношение hb между парой неатомарных чтения\записи:
 * read::   v = _buffer[cur_head]._date; в dequeue
 * write::  _buffer[cur_tail]._date = v; в enqueue
 * в случае, когда в dequeue был прочитан tail, записанный в enqueue.
 *
 * Зачем?
 * Не атомарное чтение будет считать, что в буфере содержится новая информация (например, cur_tail = cur_head + 1), но процессор мог
 * переупорядочить запись в tail и запись в буфер, тем самым мы прочитаем мусор =>
 * _tail.store(std::memory_order_release), a
 * size_t cur_tail = _tail.load(std::memory_order_acquire);
 *
 * Поскольку необходимо гарантировать hb между записью в _head в dequeue и чтением в enqueue, то для них необходимо и достаточно использовать
 * пару release/acquire, так как это самые "слабые" memory_ordering, гарантирующие sw между этими операциями.
 *
 * Остальные два чтения можно сделать relaxed, поскольку значения, которые они считывают изменяются только в тех же потоках =>
 * hb возникнет за счет po.
 */
