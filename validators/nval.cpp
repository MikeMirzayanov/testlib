/**
 * Validates that the first line contains the integer between 1 and 10^5, inclusive.
 * The second line should contains space-separated sequence of integers between -10^15 and 10^15, inclusive.
 * Also validates that file ends with EOLN and EOF.
 */

#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 100'000, "n");
    inf.readEoln();

    inf.readLongs(n, -1'000'000'000LL * 1'000'000LL, 1'000'000'000LL * 1'000'000LL, "a");
    inf.readEoln();

    inf.readEof();
}
