/*
 * Outputs random token.
 *
 * Token contains latin letters and digits and have length 
 * between 1 and 1000 characters, inclusive.
 *
 * To generate different values, call "swgen.exe <weight>".
 * See "iwgen.cpp" and "wnext()" documentation for details.
 * For example, "swgen.exe -1000" generates short strings and "swgen.exe 1000"
 * generates long strings.
 * 
 * It is typical behaviour of testlib generator to setup randseed by command line.
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int length = rnd.wnext(1, 1000, atoi(argv[1]));
    cout << rnd.next("[a-zA-Z0-9]{1,%d}", length) << endl;

    return 0;
}
