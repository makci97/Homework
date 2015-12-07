#include "SCC/SCC.h"
#include "ShortestWays/ShortestWays.h"
#include "Run.h"
#include "Fifteen/Fifteen.h"
#include <queue>


int main()
{
    std::ifstream is("test_D.txt");
    Run<int, int>(is);
    /*
    std::priority_queue<int> q;
    q.push(1);
    q.push(2);
    std::cout << q.top() << std::endl;
    */
    return 0;
}
