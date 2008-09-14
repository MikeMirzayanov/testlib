#include "testlib.h"
#include <stdio.h>
#include <math.h>

#define EPS 1.5E-5

int main(int argc, char * argv[])
{
    setName("compare two doubles, maximal absolute error = %.10lf", EPS);
    registerTestlibCmd(argc, argv);
    
    double ja = ans.readDouble();
    double pa = ouf.readDouble();
    
    if (fabs(ja - pa) > EPS)
        quitf(_wa, "expected %.10lf, found %.10lf", ja, pa);
    
    quitf(_ok, "answer is %.10lf", ja);
}
