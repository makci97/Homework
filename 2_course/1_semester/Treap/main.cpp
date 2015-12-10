#include "Treap.h"


int main()
{
    //int n = 10, k = 7;
    int n, k;

    std::cin >> n >> k;
    std::cout << Josephus_problem(n, k) << std::endl;

    return 0;
}
