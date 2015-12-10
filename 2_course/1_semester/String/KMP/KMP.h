#pragma once
#include <string>
#include <vector>


void preffix_function(const std::string& str, std::vector<int>& pref)
{
    int length = str.size();
    pref.resize(length);

    pref[0] = 0;

    int k = 0;
    for(int i = 1; i < length; ++i)
    {
        while(k > 0 && str[k] != str[i])
            k = pref[k - 1];
        if(str[k] == str[i])
            ++k;

        pref[i] = k;
    }
}

//what, where
long int KMP(const std::string& str, const std::string& text)
{
    std::vector<int> pref;

    preffix_function(str, pref);

    int j = 0;
    int k = 0;
    int size_str = str.size();
    int size_text = text.size();

    while (j + k < size_text)
    {
        if(str[k] == text[j + k])
            ++k;
        else
        {
            j += k - pref[k] + 1;
            k = pref[k];
        }

        if(k == size_str)
            break;
    }

    if(k == size_str)
        return j;

    return -1;
}
