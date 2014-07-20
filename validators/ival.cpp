/**
 * Validates that input contains the only integer between 1 and 100, inclusive.
 * Also validates that file ends with EOLN and EOF.
 */

#include "testlib.h"

using namespace std;

int main()
{
    registerValidation();

    inf.readInt(1, 100, "n");
    inf.readEoln();
    inf.readEof();

    return 0;
}
