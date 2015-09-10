#include <iostream>
#include <vector>
#include "Cache_.h"

int f(int x) {
return x==1 ? 1 : x * f(x-1);
}

int main()
{
    CachedFunction<int, int> c(f, 100);
    std::cout << c.f(5) << std::endl;
    std::cout << c.f(5) << std::endl;
    std::cout << c.f(2) << std::endl;
    std::cout << c.f(5) << std::endl;
    std::cout << c.f(2) << std::endl;
    return 0;
}
