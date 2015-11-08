#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <utility>
#include "Graph.h"
#include "Fifteen.h"


std::map<position, ptr> created;

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


void position::GetNeighbors(v_ptr &neighbors) const
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
    switch(move){
    case Up:
        if(_pos_zero - 4 < 0)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero - 4]);
        return true;
        break;
    case Down:
        if(_pos_zero + 4 > 15)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero + 4]);
        return true;
        break;
    case Left:
        if(_pos_zero % 4 == 0)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero - 1]);
        return true;
        break;
    case Right:
        if( (_pos_zero + 1) % 4 == 0)
            return false;

        std::swap(_state[_pos_zero], _state[_pos_zero + 1]);
        return true;
        break;
    default:
        break;
    }
}


ptr position::GetParent()
{
    return _parent;
}

void position::SetParent(ptr parent)
{
    _parent = parent;
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

        for(int j = i + 1; j < 16; ++j)
        {
            if(start[i] > start[j])
                ++parity;

            if(start[i] == start[j])
                return false;
        }
    }

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

    created.clear();

    std::map<ptr, std::pair<int, int> > shortest_ways; //(vertex, distanse, distanse + heuristic)
    std::set<ptr> visit;
    priority_queue<std::pair<int, ptr> > queue;

    position p_start(start);
    ptr start_ptr = std::make_shared<position>(p_start);
    created[p_start] = start_ptr;
    queue.push(start_ptr);
    int size = queue.size();

    shortest_ways[start_ptr] = {0, 0};

    std::vector<int> finish;
    for(int i = 1; i < 16; ++i)
        finish.push_back(i);
    finish.push_back(0);
    position p_target(finish);
    ptr target_ptr = std::make_shared<position>(p_target);
    created[p_target] = target_ptr;

    while(--size >= 0)
    {
        ptr cur_ptr = queue.top() -> second;

        if(cur_ptr == target_ptr)
        {
            break;
        }
        queue.pop();

        if(visit.find(cur_ptr) == visit.end() )
        {

            v_ptr neighbors;
            cur_ptr -> GetNeighbors(neighbors);

            for_each(neighbors.begin(), neighbors.end(), [&](const ptr nbr)
            {
                if(shortest_ways.find(nbr) == shortest_ways.end() ||
                   shortest_ways[cur_ptr].first + 1 < shortest_ways[nbr].first)
                {
                    shortest_ways[nbr] = std::make_pair(shortest_ways[cur_ptr].first + 1,
                                                        shortest_ways[cur_ptr].first + 1 +
                                                        nbr -> Heuristic() );

                    queue.push(std::make_pair(shortest_ways[cur_ptr].first + 1 +
                                              nbr -> Heuristic(), nbr) );

                    nbr -> SetParent(cur_ptr);
                }
            });

            visit.insert(cur_ptr);
        }

    }

}
