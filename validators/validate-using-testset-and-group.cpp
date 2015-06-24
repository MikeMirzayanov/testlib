/**
 * Validates that input depenging on testset and group.
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerValidation(argc, argv);

    int n, m;

    if (validator.testset() == "pretests")
    {
        n = inf.readInt(1, 10, "n");
        inf.readSpace();
        m = inf.readInt(1, 10, "m");
    }
    else
    {
        n = inf.readInt(1, 100, "n");
        inf.readSpace();
        m = inf.readInt(1, 100, "m");
    }

    if (validator.group() == "even-n-and-m")
    {
        ensure(n % 2 == 0);
        ensure(m % 2 == 0);
    }

    inf.readEoln();
    inf.readEof();
}
