#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-6;

int main(int argc, char * argv[])
{
    setName("compare two sequences of doubles, max absolute or relative  error = %.7lf", EPS);
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
            quitf(_wa, "%d%s numbers differ - expected: '%.7lf', found: '%.7lf', error = '%.7lf'",
                n, englishEnding(n).c_str(), j, p, doubleDelta(j, p));
        }
    }

    quitif(n == 1, _ok, "found '%.7lf', expected '%.7lf', error '%.7lf'",
        p, j, doubleDelta(j, p));
    quitf(_ok, "%d numbers", n);
}
