#include "Treap.h"


int main()
{
    //int n = 9, k = 3;
    int n, k;

    std::cin >> n >> k;
    std::cout << Josephus_problem(n, k) << std::endl;

    return 0;
}
