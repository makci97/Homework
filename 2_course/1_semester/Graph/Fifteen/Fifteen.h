#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <vector>

class position;

enum route{Up, Down, Left, Right};


typedef std::shared_ptr<position> ptr;
typedef std::vector<ptr> v_ptr;


class position
{
public:
    position() {}
    position(const std::vector<int>& state);
    position(const std::vector<int>& state, int pos_zero):
            _state(state), _pos_zero(pos_zero) {}

    inline int Heuristic() const;
    inline void GetNeighbors(v_ptr& neighbors, std::map<position, ptr>& created) const;
    inline bool Move(route move);
    inline int GetPositionOfZero() const;
    inline ptr GetParent();
    inline void SetParent(ptr parent);

    inline bool operator<(const position& other) const;
    inline bool operator==(const position& other) const;

private:
    std::vector<int> _state;
    int _pos_zero;
    ptr _parent;
};

bool solvability(std::vector<int>& start);

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
        if(_state[i] != 0)
        {
            heuristic += std::abs(i/4 - (_state[i]-1)/4);
            heuristic += std::abs(i%4 - (_state[i]-1)%4);
        }
        else
        {
            heuristic += std::abs(i/4 - 3);
            heuristic += std::abs(i%4 - 3);
        }
    }

    return heuristic*16;
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
                created.insert(std::make_pair(buf, buf_ptr) );
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

        std::swap(_state[_pos_zero], _state[_pos_zero - 4]);
        _pos_zero -= 4;
        return true;
        break;
    case Down:
        if(_pos_zero + 4 > 15)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero + 4]);
        _pos_zero += 4;
        return true;
        break;
    case Left:
        if(_pos_zero % 4 == 0)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero - 1]);
        _pos_zero -= 1;
        return true;
        break;
    case Right:
        if( (_pos_zero + 1) % 4 == 0)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero + 1]);
        _pos_zero += 1;
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

template< template<
                       class P,
                       class Container = std::vector<P>,
                       class Compare = std::greater<typename Container::value_type>
                  > class priority_queue
         >
void Fifteen(std::vector<int>& start)
{
    if( !solvability(start) )
    {
        std::cout << "-1";
        return ;
    }

    std::map<position, ptr> created;

    std::map<ptr, std::pair<int, int> > shortest_ways; //(vertex, distanse, distanse + heuristic)
    std::set<ptr> visit;
    priority_queue<std::pair<int, ptr> > queue;

    position p_start(start);
    ptr start_ptr = std::make_shared<position>(p_start);
    created[p_start] = start_ptr;
    queue.push(std::make_pair(0, start_ptr) );
    int size = queue.size();

    shortest_ways[start_ptr] = {0, 0};

    ptr cur_ptr;

    while(queue.size() >= 0)
    {
        cur_ptr = queue.top().second;

        if(cur_ptr -> Heuristic() == 0)
        {
            break;
        }
        queue.pop();

        if(visit.find(cur_ptr) == visit.end() )
        {

            v_ptr neighbors;
            cur_ptr -> GetNeighbors(neighbors, created);

            for_each(neighbors.begin(), neighbors.end(), [&](ptr nbr)
            {
                if(shortest_ways.find(nbr) == shortest_ways.end() ||
                   shortest_ways[cur_ptr].first + 1 < shortest_ways[nbr].first)
                {
                    std::pair<int, int> sh_way = std::make_pair(shortest_ways[cur_ptr].first + 1,
                                                                shortest_ways[cur_ptr].first + 1 +
                                                                nbr -> Heuristic() );
                    shortest_ways[nbr] = sh_way;

                    std::pair<int, ptr> new_state = std::make_pair(shortest_ways[cur_ptr].first + 1 +
                                                                   nbr -> Heuristic(), nbr);
                    queue.push(new_state);

                    nbr -> SetParent(cur_ptr);
                }
            });

            visit.insert(cur_ptr);
        }

    }

    std::stack<int> way;
    while(cur_ptr != start_ptr)
    {
        way.push(cur_ptr -> GetPositionOfZero() );
        cur_ptr = cur_ptr -> GetParent();
    }

    way.push(cur_ptr -> GetPositionOfZero() );
    std::cout << way.size() << std::endl;

    while( !way.empty() )
    {
        std::cout << way.top() << std::endl;
        way.pop();
    }

}
