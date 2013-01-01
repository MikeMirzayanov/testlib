#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1.5E-5;

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
        if (fabs(j - p) > EPS + 1E-15)
            quitf(_wa, "%d%s numbers differ - expected: '%.10lf', found: '%.10lf'", n, englishEnding(n).c_str(), j, p);
    }

    quitf(_ok, "%d numbers", n);
}
