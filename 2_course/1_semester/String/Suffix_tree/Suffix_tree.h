#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

//rules for special actions
enum rules{from_root, suffix_link, from_other};

struct suffix_node;
struct active_point;

struct suffix_edge
{
    suffix_edge(): _begin(0), _end(-1), _to(NULL) {}
    suffix_edge(long int beg, long int end):
        _begin(beg), _end(end), _to(NULL) {}

    inline void set_begin(long int beg);
    inline void set_end(long int end);
    inline void set_char(char c);
    inline void set_to(suffix_node* node);

    inline long int get_begin() const;
    inline long int get_end() const;
    inline char get_char() const;
    inline suffix_node* get_to();

    inline long int get_length(const std::string& text) const;

    long int _begin;    //first index of suffix in the text
    long int _end;      //second index of suffix in the text
                        //ptr for quick update all lists of tree
    char _first_char;   //first char in this edge
    suffix_node* _to;   //ptr on end node of this edge
};

struct suffix_node
{
    suffix_node():_s_link(NULL) {}

    inline void set_link(suffix_node* link);

    inline suffix_node* get_link();
    suffix_node* get_neighbour(char c);
    suffix_edge get_edge(char c);       //you must check edge before useing
    inline long int get_length(char c, const std::string& text) const;

    bool push_edge(suffix_edge edge);
    bool check_edge(char c) const;
    void rewrite_edge(char c, suffix_edge edge);
    /*
    suffix_node* travel(char c);
    */

    std::map<char, suffix_edge> _edges; //edges from this node for chars
    suffix_node* _s_link;               //suffix link
};

struct active_point
{
    inline void set_point(suffix_node* node, char edge, long int length);
    inline void set_node(suffix_node* node);
    inline void set_edge(char c);
    inline void set_length(long int length);

    inline suffix_node* get_node();
    inline char get_char() const;
    inline suffix_edge get_edge() const;
    inline long int get_length() const;

    //check and update traveler's position
    bool prepare_traveler(const std::string &text);

    suffix_node* _active_node;
    char _active_edge;
    long int _active_length;
};

class suffix_tree
{
public:
    suffix_tree(const std::string& str);
    ~suffix_tree();

    //return true if str included in _text else false with -1
    //if true then return index of first str's char
    std::pair<bool, long int> find(const std::string& str);
    //greatest suffix of each preffix of str
    //std::vector<std::pair<int, int> > gcs(const std::string& str);

private:
    std::string _text;      //text for building tree
    active_point _point;    //current position in tree
    suffix_node* _root;     //root of tree
    long int _remainder;    //amount of remains suffix

    void rule(rules flag);      //one of three special actions
    void insert(char cur_char, suffix_node *&last_added); //insert node in _point
    void add_char(char c);      //one stage of creation
    bool trip(active_point& travaler, char c);  //move traveler towards c
    void delete_node(suffix_node* node);
};
