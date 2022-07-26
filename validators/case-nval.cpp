/**
 * Validates t (1 <= t <= 10) test cases.
 * The first line contains the integer between 1 and 10^4, inclusive.
 * The second line should contains space-separated sequence of integers between -1000 and 1000, inclusive.
 * Also validates that file ends with EOLN and EOF.
 */

#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int testCaseCount = inf.readInt(1, 10, "t");
    inf.readEoln();

    for (int testCase = 1; testCase <= testCaseCount; testCase++) {
        setTestCase(testCase);
        int n = inf.readInt(1, 10000, "n");
        inf.readEoln();
        inf.readInts(n, -1000, 1000, "a");
        inf.readEoln();
    }

    inf.readEof();
}
