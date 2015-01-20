#include "testlib.h"
#include <stdio.h>
#include <math.h>

const double EPS = 1.5E-6;

int main(int argc, char * argv[])
{
    setName("compare two doubles, maximal absolute error = %.10f", EPS);
    registerTestlibCmd(argc, argv);
    
    double ja = ans.readDouble();
    double pa = ouf.readDouble();

    if (fabs(ja - pa) > EPS + 1E-15)
        quitf(_wa, "expected %.10f, found %.10f", ja, pa);
    
    quitf(_ok, "answer is %.10f", ja);
}
