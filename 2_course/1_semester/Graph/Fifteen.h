#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

enum route{Up, Down, Left, Right};

class position;

typedef std::shared_ptr<position> ptr;
typedef std::vector<ptr> v_ptr;


class position
{
public:
    position() {}
    position(const std::vector<int>& state);
    position(const std::vector<int>& state, int pos_zero):
            _state(state), _pos_zero(pos_zero) {}

    int Heuristic() const;
    void GetNeighbors(v_ptr& neighbors) const;
    bool Move(route move);
    ptr GetParent();
    void SetParent(ptr parent);

private:
    std::vector<int> _state;
    int _pos_zero;
    ptr _parent;
};
