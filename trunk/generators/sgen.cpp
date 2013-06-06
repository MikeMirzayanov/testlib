/*
 * Outputs random token.
 *
 * Token contains latin letters and digits and have length 
 * between 1 and 1000 characters, inclusive.
 *
 * To generate different values, call "sgen.exe" with different parameters.
 * 
 * It is typical behaviour of testlib generator to setup randseed by command line.
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    cout << rnd.next("[a-zA-Z0-9]{1,1000}") << endl;

    return 0;
}
