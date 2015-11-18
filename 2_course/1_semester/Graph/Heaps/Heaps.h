#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

class position;

enum route{Up, Down, Left, Right};


typedef std::shared_ptr<position> ptr;
typedef std::vector<ptr> v_ptr;

//extern std::map<position, ptr> created;


class position
{
public:
    position() {}
    position(const std::vector<int>& state);
    position(const std::vector<int>& state, int pos_zero):
            _state(state), _pos_zero(pos_zero) {}

    int Heuristic() const;
    void GetNeighbors(v_ptr& neighbors, std::map<position, ptr>& created) const;
    bool Move(route move);
    ptr GetParent();
    void SetParent(ptr parent);

    bool operator<(const position& other) const;

private:
    std::vector<int> _state;
    int _pos_zero;
    ptr _parent;
};

bool solvability(std::vector<int>& start);

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

    std::vector<int> finish;
    for(int i = 1; i < 16; ++i)
        finish.push_back(i);
    finish.push_back(0);
    position p_target(finish);
    ptr target_ptr = std::make_shared<position>(p_target);
    created[p_target] = target_ptr;

    while(--size >= 0)
    {
        ptr cur_ptr = queue.top().second;

        if(cur_ptr == target_ptr)
        {
            break;
        }
        queue.pop();

        if(visit.find(cur_ptr) == visit.end() )
        {

            v_ptr neighbors;
            cur_ptr -> GetNeighbors(neighbors, created);

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
