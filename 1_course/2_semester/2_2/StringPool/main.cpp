#include <iostream>
#include "StringPool.h"

int main()
{
    int n = 10, m = 12;
    StringPool sp(n);
    char c = 'a';
    std::string s;
    for(int i = 0; i < m; ++i)
    {
        s = c;
        try
        {
            sp.GetId(s);
        }
        catch(std::logic_error)
        {
            break;
        }

        ++c;
    }
    c = 'c';
    for(int i = 0; i < m; ++i)
    {
        s = c;
        try
        {
            std::cout << "id = " << sp.GetId(s) << std::endl;
        }
        catch(std::logic_error)
        {
            break;
        }

        ++c;
    }
    for(int i = 0; i < m; ++i)
    {
        try
        {
            std::cout << "id = " << i << "\tword =  " << sp.GetWord(i) << std::endl;
        }
        catch(...)
        {
            break;
        }
    }
    return 0;
}
