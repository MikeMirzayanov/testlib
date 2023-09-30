#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int t = inf.readInt(1, 3, "t");
    inf.readEoln();

    for (int tt = 1; tt <= t; tt++) {
        setTestCase(tt + 1);
        
        int n = inf.readInt(1, 4, "n");
        inf.readSpace();
        inf.readInt(-2, 2, "l0");
        inf.readSpace();
        inf.readInt(-5, 5, "r[1]");
        inf.readEoln();

        vector<int> a = inf.readInts(n, 1, n, "a");
        ensuref(int(set<int>(a.begin(), a.end()).size()) == n, "not a permutation (duplicates)");
        inf.readEoln();
    }

    inf.readEof();
}
