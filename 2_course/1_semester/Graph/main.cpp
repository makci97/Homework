#include <iostream>
#include <fstream>
#include "Run.h"


int main()
{
    //std::ifstream is("test_MST.txt");
    //Run<int, int>(is);

    Run<int, int>(std::cin);

    return 0;
}
