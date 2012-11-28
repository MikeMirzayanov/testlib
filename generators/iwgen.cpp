/*
 * Outputs weighted random number between 1 and 10^6, inclusive.
 * To generate different values, call "nwgen.exe weight". 
 *
 * If parameter "weight" 
 * is equals to 0 than used uniformly distributed random.
 *
 * If parameter "weight" > 0 then you can think about it as code like this:
 * <code>
 * result = rnd.next(1, 1000000);
 * for (int i = 0; i < weight; i++)
 *     result = max(result, rnd.next(1, 1000000);
 * </code> 
 * 
 * If parameter "weight" < 0 then you can think about it as code like this:
 * <code>
 * result = rnd.next(1, 1000000);
 * for (int i = 0; i < -weight; i++)
 *     result = min(result, rnd.next(1, 1000000);
 * </code> 
 *
 * It is typical behaviour of "wnext" methods to use this strategy to 
 * generate off-center random distribution.
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv);

    cout << rnd.wnext(1, 1000000, atoi(argv[1])) << endl;

    return 0;
}
