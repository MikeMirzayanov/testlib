/*
 * It is another type of generators. It writes several files named 
 * as test indices. 
 *
 * For example, this generator writes 10 files (tests) from 1 to 10.
 * This type of generators is supported by Polygon too, but
 * stdout-generators are more preferred.
 *
 * The generator is for A+B problem, generates 10 tests where each 
 * number is between 1 and 100, and tests grow with indices.
 */

#include "testlib.h"

using namespace std;

void writeTest(int test) {
    startTest(test);

    int a = rnd.next(1, test * test);
    int b = rnd.next(1, test * test);
    println(a, b);
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    for (int i = 1; i <= 10; i++)
        writeTest(i);
}
