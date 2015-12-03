#include <iostream>
#include "Suffix_tree.h"


int main()
{
    //std::ifstream is("test_suffix_tree.txt");

    std::string str = "abcabxabcd";
    suffix_tree tree(str);
    std::cout<< tree.find("abcd").second << std::endl;
    return 0;
}
