#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-9;

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
    setName("compare two sequences of doubles, max absolute or relative error = %.10lf", EPS);
    registerTestlibCmd(argc, argv);

    int n = 0;
    double j, p;

    while (!ans.seekEof()) 
    {
      n++;
      j = ans.readDouble();
      p = ouf.readDouble();
      if (!doubleCompare(j, p, EPS))
        quitf(_wa, "%d%s numbers differ - expected: '%.10lf', found: '%.10lf', error = '%.10lf'", n, ending(n).c_str(), j, p, doubleDelta(j, p));
    }

    quitif(n == 1, _ok, "found '%.10lf', expected '%.10lf', error '%.10lf'",
        p, j, doubleDelta(j, p));
    quitf(_ok, "%d numbers", n);
}
