#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-4;

int main(int argc, char * argv[])
{
    setName("compare two sequences of doubles, max absolute or relative error = %.5lf", EPS);
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
            quitf(_wa, "%d%s numbers differ - expected: '%.5lf', found: '%.5lf', error = '%.5lf'",
                n, englishEnding(n).c_str(), j, p, doubleDelta(j, p));
        }
    }

    quitif(n == 1,
        _ok, "found '%.5lf', expected '%.5lf', error '%.5lf'",
        p, j, doubleDelta(j, p));
    
    quitf(_ok, "%d numbers", n);
}
