#include <iostream>
#include "Suffix_tree.h"


int main()
{
    //std::ifstream is("test_suffix_tree.txt");"abcabxabcd";
    std::string text, str;
    std::cin >> text;
    std::cin >> str;
    suffix_tree tree(text);
    std::cout<< tree.find(str).second << std::endl;
    return 0;
}
