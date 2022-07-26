/**
 * Validates that the input contains the only token token.
 * This token can contain only lowercase latin letters a-z. The length should be between 1 and 100, inclusive.
 * Also validates that file ends with EOLN and EOF.
 */

#include "testlib.h"

using namespace std;

int main(int argc, char* argv[])
{
    registerValidation(argc, argv);
    
    inf.readToken("[a-z]{1,100}", "s");
    inf.readEoln();
    inf.readEof();

    return 0;
}
