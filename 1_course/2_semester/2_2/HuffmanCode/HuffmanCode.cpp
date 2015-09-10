#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <utility>
#include <queue>
#include <map>
#include <algorithm>
#include <memory>
#include "HuffmanCode.h"

//*************************************************************************
//Class NodeCmp
//*************************************************************************

bool NodeCmp::operator()(const std::shared_ptr<Node> first, const std::shared_ptr<Node> second)
{
    return first -> frequency > second -> frequency;
}

//*************************************************************************
//Class HuffmanCode
//*************************************************************************

std::vector<char> HuffmanCode::Code(const std::vector<char>& text)
{
    //std::cout << "0" << std::endl;
    this -> DeterminationOfFrequency(text);
   // std::cout << "1 " << _frequency.size() << std::endl;
    this -> BuildTree();
   // std::cout << "2" << std::endl;
    this -> BuildCoder(_head_of_tree);
    //std::cout << "3" << std::endl;

    auto it = _coder.begin(), end = _coder.end();
    while(it != end)
    {
        //std::cout << it -> first << std::endl;
        ++it;
    }

    std::vector<char> code;
    char c;
    for(auto it_text = text.begin(), end_text = text.end(); it_text != end_text; ++it_text)
    {
        auto it_bool = _coder.find(*it_text) -> second.begin(), end_bool = _coder.find(*it_text) -> second.end();
        while(it_bool != end_bool)
        {
            if(*it_bool)
                c = '1';
            else
                c = '0';
            code.push_back(c);
            ++it_bool;
        }
    }
    //std::cout << "4" << std::endl;
    return code;
}

std::vector<char> HuffmanCode::Decode(const std::vector<char>& code)
{
    std::vector<char> text;
    std::vector<bool> buf;
    auto it_code = code.begin(), end_code = code.end();
    auto end_decoder = _decoder.end();
    while(it_code != end_code)
    {
        if(*it_code == '1')
            buf.push_back(true);
        else
            buf.push_back(false);
        auto it_decoder = _decoder.find(buf);
        if(it_decoder != end_decoder)
        {
            text.push_back(it_decoder -> second);
            buf.clear();
        }
        ++it_code;
    }

    return text;
}

std::map<char, std::vector<bool> > HuffmanCode::GetCoder()
{
    return _coder;
}

void HuffmanCode::SetCoder(const std::map<char, std::vector<bool> >& coder)
{
    _coder = coder;
}

std::vector<char> HuffmanCode::GetDecoder()
{
    std::vector<char> result;
    char c;
    auto it = _decoder.begin(), end = _decoder.end();
    while(it != end)
    {
        auto it_vec = it -> first.begin(), end_vec = it -> first.end();
        while(it_vec != end_vec)
        {
            if(*it_vec)
                c = '1';
            else
                c = '0';
            result.push_back(c);
            ++it_vec;
        }
        result.push_back(' ');
        c = it -> second;
        result.push_back(c);
        result.push_back('\n');
        ++it;
    }
    return result;
}

void HuffmanCode::SetDecoder(const std::vector<char>& decoder)
{
    auto it = decoder.begin(), end = decoder.end();
    std::vector<bool> buf_vec;
    while(it != end && *it != EOF)
    {
        buf_vec.clear();
        while(*it != ' ' && it != end)
        {
            if(*it == '1')
                buf_vec.push_back(true);
            else
                buf_vec.push_back(false);
            ++it;
        }
        ++it;
        _decoder[buf_vec] = *it;
        std::cout << *it << std::endl;

        ++it;
        while(*it != '\n' && it != end)
            ++it;
        ++it;
        std::cout << "good" << std::endl;
    }
}


void HuffmanCode::DeterminationOfFrequency(const std::vector<char>& text)
{
    auto it_text = text.begin(), end_text = text.end();
    while(it_text != end_text)
    {
        auto it_freq = _frequency.find(*it_text);
        if(it_freq == _frequency.end())
            _frequency[*it_text] = 1;
        else
        {
            ++_frequency[*it_text];
        }

        ++it_text;
    }

}

void HuffmanCode::BuildTree()
{
    auto it_freq = _frequency.begin(), end_freq = _frequency.end();
    while(it_freq != end_freq)
    {
        std::shared_ptr<Node> new_node(new Node);
        new_node -> c = it_freq -> first;
        new_node -> frequency = it_freq -> second;
        _for_tree.push(new_node);
        ++it_freq;
    }

    unsigned long int size = _for_tree.size();
    while(--size >= 1)
    {
        std::shared_ptr<Node> left_child(new Node);
        left_child = _for_tree.top();
        _for_tree.pop();
        std::shared_ptr<Node> right_child(new Node);
        right_child = _for_tree.top();
        _for_tree.pop();
        std::shared_ptr<Node> parent(new Node);
        parent -> left = left_child;
        parent -> right = right_child;
        parent -> frequency = left_child -> frequency + right_child -> frequency;

        _for_tree.push(parent);
    }

    _head_of_tree = _for_tree.top();
}

void HuffmanCode::BuildCoder(const std::shared_ptr<Node> _head_of_tree)
{
    static std::vector<bool> code;
    if(_head_of_tree -> left != nullptr)
    {
        code.push_back(0);
        BuildCoder(_head_of_tree -> left);
    }
    if(_head_of_tree -> right != nullptr)
    {
        code.push_back(1);
        BuildCoder(_head_of_tree -> right);
    }
    if(_head_of_tree -> left == nullptr && _head_of_tree -> right == nullptr)
    {
        _coder[_head_of_tree -> c] = code;
        _decoder[code] = _head_of_tree -> c;
    }
    code.pop_back();
}
