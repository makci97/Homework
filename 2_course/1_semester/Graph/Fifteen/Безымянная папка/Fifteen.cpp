#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <utility>
#include "Fifteen.h"

position::position(const std::vector<int>& state):_state(state)
{
    int i = 0;
    while(i < 16 && state[i] != 0)
        ++i;

    if(i < 16)
        _pos_zero = i;
}

int position::Heuristic() const
{
    int heuristic = 0;

    for(int i = 0; i < 16; ++i)
    {
        heuristic += std::abs(i/4 - _state[i]/4);
        heuristic += std::abs(i%4 - _state[i]%4);
    }

    return heuristic;
}


void position::GetNeighbors(v_ptr &neighbors, std::map<position, ptr>& created) const
{
    position buf = *this;
    neighbors.clear();

    route commands[] = {Up, Down, Left, Right};
    for(int i = 0; i < 4; ++i)
    {
        if( buf.Move(commands[i]) )
        {
            auto it = created.find(buf);

            if(it == created.end() )
            {
                ptr buf_ptr = std::make_shared<position>(buf);
                neighbors.push_back(buf_ptr);
                //std::cout << buf_ptr ->GetPositionOfZero() << std::endl;
                created.insert(std::make_pair(buf, buf_ptr) );
                //created[buf] = buf_ptr;
            }
            else
            {
                neighbors.push_back(it -> second);
            }
        }

        buf = *this;
    }
}

bool position::Move(route move)
{
    int buf1, buf2;
    switch(move){
    case Up:
        if(_pos_zero - 4 < 0)
            return false;

        //buf1 = _state[_pos_zero] - _state[_pos_zero - 4];
        std::swap(_state[_pos_zero], _state[_pos_zero - 4]);
        //buf2 = _state[_pos_zero] - _state[_pos_zero - 4];
        _pos_zero -= 4;
        //std::cout << " d_buf Up = " << buf1+buf2 << std::endl;
        return true;
        break;
    case Down:
        if(_pos_zero + 4 > 15)
            return false;

        //buf1 = _state[_pos_zero] - _state[_pos_zero + 4];
        std::swap(_state[_pos_zero], _state[_pos_zero + 4]);
        //buf2 = _state[_pos_zero] - _state[_pos_zero + 4];
        _pos_zero += 4;
        //std::cout << " d_buf Down = " << buf1+buf2 << std::endl;
        return true;
        break;
    case Left:
        if(_pos_zero % 4 == 0)
            return false;

        //buf1 = _state[_pos_zero] - _state[_pos_zero - 1];
        std::swap(_state[_pos_zero], _state[_pos_zero - 1]);
        //buf2 = _state[_pos_zero] - _state[_pos_zero - 1];
        _pos_zero -= 1;
        //std::cout << " d_buf Left = " << buf1+buf2 << std::endl;
        return true;
        break;
    case Right:
        if( (_pos_zero + 1) % 4 == 0)
            return false;

        //buf1 = _state[_pos_zero] - _state[_pos_zero + 1];
        std::swap(_state[_pos_zero], _state[_pos_zero + 1]);
        //buf2 = _state[_pos_zero] - _state[_pos_zero + 1];
        _pos_zero += 1;
        //std::cout << " d_buf Right = " << buf1+buf2 << std::endl;
        return true;
        break;
    default:
        break;
    }
}


int position::GetPositionOfZero() const
{
    return _pos_zero;
}

ptr position::GetParent()
{
    return _parent;
}

void position::SetParent(ptr parent)
{
    _parent = parent;
}

bool position::operator<(const position& other) const
{
    return _state < other._state;
}

bool position::operator==(const position& other) const
{
    return _state == other._state;
}

//if not work, then rewrite for zero
bool solvability(std::vector<int>& start)
{
    if(start.size() != 16)
        return false;

    int parity = 0;

    for(int i = 0; i < 16; ++i)
    {
        if(start[i] > 15 || start[i] < 0)
            return false;

        if(start[i] != 0)
        {
            for(int j = i + 1; j < 16; ++j)
            {
                if(start[i] > start[j] && start[j] != 0)
                    ++parity;

                if(start[i] == start[j])
                    return false;
            }
        }
        else
        {
            parity += i/4 + 1;
        }
    }

    std::cout << parity << std::endl;
    if(parity % 2 == 0)
        return true;

    return false;
}
