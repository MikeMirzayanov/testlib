#include "testlib.h"
#include <cmath>

using namespace std;

#define EPS 1.5E-5

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
    setName("compare two sequences of doubles, maximal absolute error = %.10lf", EPS);
    registerTestlibCmd(argc, argv);

    int n = 0;
    while (!ans.seekEof()) 
    {
      n++;
      double j = ans.readDouble();
      double p = ouf.readDouble();
      if (fabs(j - p) > EPS)
        quitf(_wa, "%d%s numbers differ - expected: '%.10lf', found: '%.10lf'", n, ending(n).c_str(), j, p);
    }

    quitf(_ok, "%d numbers", n);
}
