#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-6;

int main(int argc, char * argv[])
{
    setName("compare two sequences of doubles, max absolute or relative  error = %.7f", EPS);
    registerTestlibCmd(argc, argv);

    int n = 0;
    double j, p;

    while (!ans.seekEof()) 
    {
        n++;
        j = ans.readDouble();
        p = ouf.readDouble();
        if (!doubleCompare(j, p, EPS))
        {
            quitf(_wa, "%d%s numbers differ - expected: '%.7f', found: '%.7f', error = '%.7f'",
                n, englishEnding(n).c_str(), j, p, doubleDelta(j, p));
        }
    }

    if (n == 1)
        quitf(_ok, "found '%.7f', expected '%.7f', error '%.7f'", p, j, doubleDelta(j, p));

    quitf(_ok, "%d numbers", n);
}
