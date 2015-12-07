#include "SCC.h"
#include "../ShortestWays/ShortestWays.h"
#include "../Run.h"
#include "../Fifteen/Fifteen.h"


int main()
{
    std::ifstream is("test_FW.txt");
    Run<int, int>(is);
    return 0;
}
