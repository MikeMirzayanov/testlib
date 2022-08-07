#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int testCaseCount = inf.readInt(1, 10, "t");
    inf.readEoln();

    for (int testCase = 1; testCase <= testCaseCount; testCase++) {
        setTestCase(testCase - 1);
        inf.readLine("[\\\\!a-z]{1,100}");
    }

    inf.readEof();
}
