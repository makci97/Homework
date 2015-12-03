#include <algorithm>
#include <cassert>
#include "Suffix_tree.h"

//***************************************************************************
//suffix_edge
//***************************************************************************

inline void suffix_edge::set_begin(long int beg)
{
    _begin = beg;
}

inline void suffix_edge::set_end(long int end)
{
    _end = end;
}

inline void suffix_edge::set_char(char c)
{
    _first_char = c;
}

inline void suffix_edge::set_to(suffix_node* node)
{
    _to = node;
}

inline long int suffix_edge::get_begin() const
{
    return _begin;
}

inline long int suffix_edge::get_end() const
{
    return _end;
}

inline char suffix_edge::get_char() const
{
    return _first_char;
}

inline suffix_node* suffix_edge::get_to()
{
    return _to;
}

inline long int suffix_edge::get_length(const std::string& text) const
{
    return (_end == -1) ? text.size() : (_end - _begin);
}


//***************************************************************************
//suffix_node
//***************************************************************************

inline void suffix_node::set_link(suffix_node* link)
{
    _s_link = link;
}

inline suffix_node* suffix_node::get_link()
{
    return _s_link;
}

suffix_node* suffix_node::get_neighbour(char c)
{
    auto it = _edges.find(c);

    if(it != _edges.end())
    {
        return it->second.get_to();
    }

    return this;
}

suffix_edge suffix_node::get_edge(char c)
{
    auto it = _edges.find(c);
    if(it != _edges.end())
        return it->second;
}

inline long int suffix_node::get_length(char c, const std::string& text) const
{
    auto it = _edges.find(c);
    if(it != _edges.end())
        return it->second.get_length(text);

    return 0;
}

bool suffix_node::push_edge(suffix_edge edge)
{
    if(edge.get_begin() >= 0 && !this->check_edge(edge.get_char()) )
    {
        edge.set_end(-1);
        edge.set_to(NULL);
        _edges[edge.get_char()] = edge;
    }
}

bool suffix_node::check_edge(char c) const
{
    if(_edges.find(c) == _edges.end())
        return false;

    return true;
}

void suffix_node::rewrite_edge(char c, suffix_edge edge)
{
    auto it = _edges.find(c);
    if(it != _edges.end())
        _edges.erase(it);

    _edges[c] = edge;
}

//***************************************************************************
//active_point
//***************************************************************************


inline void active_point::set_point(suffix_node *node, char edge, long int length)
{
    _active_node = node;
    _active_edge = edge;
    _active_length = length;
}

inline void active_point::set_node(suffix_node *node)
{
    _active_node = node;
}

inline void active_point::set_edge(char c)
{
    _active_edge = c;
}

inline void active_point::set_length(long int length)
{
    _active_length = length;
}

inline suffix_node* active_point::get_node()
{
    return _active_node;
}

inline char active_point::get_char() const
{
    return _active_edge;
}

inline suffix_edge active_point::get_edge() const
{
    return _active_node->get_edge(_active_edge);
}

inline long int active_point::get_length() const
{
    return _active_length;
}

bool active_point::prepare_traveler(const std::string& text)
{
    if(_active_edge != '\0' &&
       this->get_edge().get_begin() + _active_length >= text.size()
      )
    {
        //travaler leave our space
        return false;
    }

    bool flag = true;

    if(_active_node->check_edge(_active_edge) &&
        _active_node->get_length(_active_edge, text)
        <= _active_length
      )
    {
        //bad state of active_point
        suffix_node *node = _active_node;
        long int length = _active_length;
        char c = _active_edge;

        while(node->get_length(c, text) <= length)
        {
            length -= node->get_length(c, text);
            node = node->get_edge(c).get_to();
            assert(node != NULL);
            if(length > 0)
                c = text[text.size() - length];
            else
            {
                c = '\0';
                break;
            }
        }

        this->set_point(node, c, length);
    }

    return flag;
}

//***************************************************************************
//suffix_tree
//***************************************************************************

suffix_tree::suffix_tree(const std::string& str)
{
    _root = new suffix_node;
    _point.set_point(_root, '\0', 0);
    _remainder = 0;

    for(auto it = str.begin(), end = str.end();
        it != end; ++it)
    {
        add_char(*it);
    }

    add_char('$');
}

suffix_tree::~suffix_tree()
{
    delete_node(_root);
}

void suffix_tree::rule(rules flag)
{

}

void suffix_tree::insert(char cur_char, suffix_node*& last_added)
{
    suffix_edge main_edge, part_of_old, new_edge;
    main_edge = _point.get_edge();
    part_of_old = main_edge;

    suffix_node *new_node = new suffix_node;
    main_edge.set_to(new_node);

    //index = after last char in main and first in part_of_old
    long int index = main_edge.get_begin() + _point.get_length();
    main_edge.set_end(index);
    new_edge.set_begin(_text.size() - 1);
    part_of_old.set_begin(index);

    new_edge.set_char(cur_char);
    part_of_old.set_char(_text[index]);

    new_node->push_edge(new_edge);
    new_node->push_edge(part_of_old);
    _point.get_node()->rewrite_edge(_point.get_char(), main_edge);

    --_remainder;

    if(_point.get_node() == _root)
    {
        //rule 1
        _point.set_point(_root, _text[_text.size() - _remainder], _point.get_length() - 1);

    }
    else
    {
        //rule 3
        suffix_node *link = _point.get_node()->get_link();
        if(link == NULL)
            link = _root;
        //bad state
        long int length = _point.get_length();
        char c = _point.get_char();
        _point.set_point(link, c, length);
    }

    if(last_added != NULL)
    {
        //rule 2
        last_added->set_link(new_node);
    }
    last_added = new_node;

    if(_remainder == 0)
        _point.set_point(_root, '\0', 0);
}

void suffix_tree::add_char(char c)
{
    ++_remainder;
    _text.push_back(c);
    bool move = 0;
    suffix_node* last_added = NULL;

    while(_remainder > 0 && !move)
    {
        /*

        if(_point.get_node()->get_length(_point.get_char(), _text)
                <= _point.get_length() &&
                _point.get_node()->check_edge(_point.get_char())
          )
        {
            //bad state of active_point
            suffix_node *node = _point.get_node();
            long int length = _point.get_length();
            char c = _point.get_char();

            while(node->get_length(c, _text) <= length)
            {
                length -= node->get_length(c, _text);
                node = node->get_edge(c).get_to();
                assert(node != NULL);
                if(length > 0)
                    c = _text[_text.size() - length];
                else
                {
                    c = '\0';
                    break;
                }
            }

            _point.set_point(node, c, length);
        }
        */
        _point.prepare_traveler(_text);
        //active_point is true for this step

        if(_point.get_length() == 0)
        {
            //in node
            if(_point.get_node()->check_edge(c))
            {
                //move active_point
                _point.set_point(_point.get_node(), c, 1);
                move = true;
            }
            else
            {
                //in root
                suffix_edge edge;
                edge.set_char(c);
                edge.set_begin(_text.size() - 1);
                _root->push_edge(edge);
                --_remainder;

                if(_remainder == 0)
                    _point.set_point(_root, '\0', 0);
            }
        }
        else
        {
            //inside edge
            assert(_point.get_node()->check_edge(_point.get_char()));
            suffix_edge edge = _point.get_edge();

            if(_text[edge.get_begin() + _point.get_length()] == c)
            {
                _point.set_length(_point.get_length() + 1);
                move = true;
            }
            else
            {
                //insert node in edge after active_point
                this->insert(c, last_added);
            }
        }
    }
}

void suffix_tree::delete_node(suffix_node* node)
{
    if(node == NULL)
        return;

    for(auto it = node->_edges.begin(), end = node->_edges.end();
       it != end; ++it)
    {
       this->delete_node(it->second._to);
    }

    delete node;
}


std::pair<bool, long int> suffix_tree::find(const std::string& str)
{
    if(str.empty() || !_root->check_edge(str[0]))
        return {false, -1};

    bool flag = true;
    active_point traveler;
    traveler.set_point(_root, '\0', 0);

    for(auto it = str.begin(), end = str.end();
        it != end; ++it)
    {
        if(!trip(traveler, *it))
        {
            flag = false;
            break;
        }
    }

    if(flag == true)
    {
        long int index = traveler.get_edge().get_begin()
                + traveler.get_length() - str.size();
        return {true, index};
    }
    else
        return {false, -1};
}
bool suffix_tree::trip(active_point& travaler, char c)
{
    //check traveler
    if(!travaler.prepare_traveler(_text))
        return false;
    /*

    if(travaler.get_node()->get_length(travaler.get_char(), _text)
            <= travaler.get_length() &&
            travaler.get_node()->check_edge(travaler.get_char())
      )
    {
        //bad state of active_point
        suffix_node *node = travaler.get_node();
        long int length = travaler.get_length();
        char c = travaler.get_char();

        while(node->get_length(c, _text) <= length)
        {
            length -= node->get_length(c, _text);
            node = node->get_edge(c).get_to();
            assert(node != NULL);
            if(length > 0)
                c = _text[_text.size() - length];
            else
            {
                c = '\0';
                break;
            }
        }

        travaler.set_point(node, c, length);
    }
    */

    //traveler ready for trip
    bool flag = true;

    if(travaler.get_char() == '\0')
    {
        //in node
        if(travaler.get_node()->check_edge(c))
        {
            travaler.set_edge(c);
            travaler.set_length(1);
        }
        else
            flag = false;
    }
    else
    {
        //inside edge
        long int index = travaler.get_edge().get_begin()
                + travaler.get_length();
        if(index < _text.size() && _text[index] == c)
            travaler.set_length(travaler.get_length() + 1);
        else
            flag = false;
    }

    return flag;
}
