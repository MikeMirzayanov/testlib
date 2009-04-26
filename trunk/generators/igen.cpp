/*
 * Outputs random number between 1 and 10^6, inclusive.
 * To generate different values, call "ngen.exe" with different parameters.
 * For example, "ngen.exe 1" returns 504077, but "ngen.exe 3" returns 808747.
 * 
 * It is typical behaviour of testlib generator to setup randseed by command line.
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv);

    cout << rnd.next(1, 1000000) << endl;

    return 0;
}
