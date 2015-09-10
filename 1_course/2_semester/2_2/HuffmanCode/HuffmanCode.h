#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <queue>
#include <map>
#include <memory>

struct Node
{
    char c;
    unsigned long int frequency;

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(): left(nullptr), right(nullptr) {}
    Node operator=(const Node& other)
    {
        c = other.c;
        frequency = other.frequency;
        left = other.left;
        right = other.right;
    }
};

class NodeCmp
{
public:
    bool operator()(const std::shared_ptr<Node> first, const std::shared_ptr<Node> second);
};

class HuffmanCode
{
public:
    HuffmanCode(){}
    std::vector<char> Code(const std::vector<char>& text);
    std::vector<char> Decode(const std::vector<char>& code);

    std::map<char, std::vector<bool> > GetCoder();
    void SetCoder(const std::map<char, std::vector<bool> >& coder);

    std::vector<char> GetDecoder();
    void SetDecoder(const std::vector<char>& decoder);

private:
    std::map<char, unsigned long int> _frequency;
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, NodeCmp> _for_tree;
    std::map<char, std::vector<bool> > _coder;
    std::map<std::vector<bool>, char> _decoder;
    std::shared_ptr<Node> _head_of_tree;

    void DeterminationOfFrequency(const std::vector<char>& text);
    void BuildTree();
    void BuildCoder(const std::shared_ptr<Node> _head_of_tree);
};
