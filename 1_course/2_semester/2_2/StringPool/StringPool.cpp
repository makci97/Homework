#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "StringPool.h"

//**************************************************************
//Class StringPool
//**************************************************************

StringPool::StringPool(int n):_n(n)
{
    _id.reserve(n);
}

int StringPool::GetId(const std::string &word)
{
    auto it_word = _word.find(word);

    if(it_word == _word.end())
    {
        if(_id.size() < _n)
        {
            _id.push_back(word);
            int size = _id.size();
            _word[word] = size - 1;
            return size - 1;
        }
        else
            throw std::logic_error("StringPool is full");
    }

    else
    {
        return it_word -> second;
    }
}

std::string StringPool::GetWord(int id) const
{
    return _id.at(id);
}
