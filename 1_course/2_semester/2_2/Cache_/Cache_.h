#pragma once
#include <iostream>
#include <list>
#include <algorithm>
#include <utility>


template<typename ArgumentType, typename ReturnedType>
class CachedFunction
{
public:
    CachedFunction(ReturnedType (*f)(ArgumentType), int m);
    ReturnedType f(ArgumentType arg);
private:
    int _m;
    ReturnedType (*_f)(ArgumentType);
    std::list<std::pair<ArgumentType, ReturnedType> > _cache;
};


//***************************************************************************
//Class CachedFunction
//***************************************************************************

template<typename ArgumentType, typename ReturnedType>
CachedFunction<ArgumentType, ReturnedType>::CachedFunction (ReturnedType (*f)(ArgumentType), int m)
{
    _m = m;
    _f = f;
}


template<typename ArgumentType, typename ReturnedType>
ReturnedType CachedFunction<ArgumentType, ReturnedType>::f(ArgumentType arg)
{
    /*auto it = std::find(_cache.begin(), _cache.end(), [&arg](const std::pair<ArgumentType, ReturnedType>& p)->bool
    {
        return p.first == arg;
    });*/
    auto it = _cache.begin(), end = _cache.end();
    while(it != end)
    {
        if(it -> first == arg)
            break;
        ++it;
    }

    if(it != end)
    {
        _cache.push_front(*it);
        _cache.erase(it);
        std:: cout << "1 ";
    }
    else
    {
        if(_cache.size() >= _m)
            _cache.pop_back();
        _cache.push_front({arg, _f(arg)});
        std:: cout << "0 ";
    }

    return _cache.begin() -> second;
}
