#include "testlib.h"

#include <set>

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 1000, "n");
    inf.readSpace();
    int m = inf.readInt(0, 100000, "m");
    inf.readEoln();

    set<pair<int, int>> edges;

    for (int i = 0; i < m; i++) {
        int a = inf.readInt(1, n, "a_i");
        inf.readSpace();
        int b = inf.readInt(1, n, "b_i");
        inf.readEoln();

        ensuref(a != b, "Graph can't contain loops");
        ensuref(edges.count(make_pair(a, b)) == 0,
                "Graph can't contain multiple edges between a pair of vertices");

        edges.insert(make_pair(a, b));
        edges.insert(make_pair(b, a));
    }

    inf.readEof();
}
