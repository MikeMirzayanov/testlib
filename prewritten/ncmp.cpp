#include "testlib.h"
#include <sstream>

using namespace std;

string ending(long long x)
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

string ltoa(long long n)
{
    char c[32];
    sprintf(c, "%I64d", n);
    return c;
}

int main(int argc, char * argv[])
{
    setName("compare ordered sequences of signed int%d numbers", 8 * sizeof(long long));

    registerTestlibCmd(argc, argv);

    int n = 0;
    
    string firstElems;
    
    while (!ans.seekEof()) 
    {
      n++;
      long long j = ans.readLong();
      long long p = ouf.readLong();
      if (j != p)
        quitf(_wa, "%d%s numbers differ - expected: '%I64d', found: '%I64d'", n, ending(n).c_str(), j, p);
      else
        if (n <= 5)
        {
            if (firstElems.length() > 0)
                firstElems += " ";
            firstElems += ltoa(j);
        }
    }

    if (n <= 5)
    {
        quitf(_ok, "%d number(s): \"%s\"", n, firstElems.c_str());
    }
    else
        quitf(_ok, "%d numbers", n);
}
