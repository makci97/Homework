#include <iostream>
#include "KMP.h"


int main()
{
    /*
    std::string str = "ab";
    std::string text = "abcabxabcd";
    */
    std::string text, str;
    std::cin >> str >> text;

    std::cout << KMP(str, text) << std::endl;
    return 0;
}
