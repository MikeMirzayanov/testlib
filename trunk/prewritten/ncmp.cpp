#include "testlib.h"

using namespace std;

string ending(int x)
{
    x %= 100;
    if (x / 10 == 1)
        return "th";
    if (x % 10 == 1)
        return "st";
    if (x % 10 == 2)
        return "nd";
    if (x % 10 == 3)
        return "rd";
    return "th";
}

int main(int argc, char * argv[])
{
    setName("compare ordered sequences of signed int%d's", 8 * sizeof(int));

    registerTestlibCmd(argc, argv);

    int n = 0;
    
    while (!ans.seekEof()) 
    {
      n++;
      int j = ans.readInt();
      int p = ouf.readInt();
      if (j != p)
        quitf(_wa, "%d%s numbers differ - expected: '%d', found: '%d'", n, ending(n).c_str(), j, p);
    }
    
    quitf(_ok, "%d numbers", n);
}
