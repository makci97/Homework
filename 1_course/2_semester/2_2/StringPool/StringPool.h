#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class StringPool
{
    public:
        StringPool(int n);
        int GetId(const std::string &word);
        std::string GetWord(int id) const;
    private:
        int _n;
        std::unordered_map<std::string, int> _word;
        std::vector<std::string> _id;
};
