#include "SCC.h"
#include "ShortestWays.h"
#include "Run.h"


int main()
{
    std::ifstream is("test_FW.txt");
    Run<int, int>(is);
    return 0;
}
