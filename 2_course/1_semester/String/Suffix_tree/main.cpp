#include <iostream>
#include "Suffix_tree.h"


int main()
{
    //std::ifstream is("test_suffix_tree.txt");
/*
    std::cout << sizeof(suffix_position) << std::endl;
    int c = 3;
    int *a = &c, &b = c;
    std::cout << *a << b << c << std::endl;
    *a = 1;
    b = 2;
    std::cout << *a << b << c << std::endl;
    //suffix_edge ed;
    suffix_node node;
    char ch = 0;
    std::cout << ch << sizeof(suffix_position) << std::endl;
*/
    std::string str = "abcabxabcd";
    suffix_tree tree(str);
    return 0;
}
