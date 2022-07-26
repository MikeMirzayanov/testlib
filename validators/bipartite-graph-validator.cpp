#include "testlib.h"

#include <set>

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 400, "n");
    inf.readSpace();
    int m = inf.readInt(1, 400, "m");
    inf.readSpace();
    int k = inf.readInt(0, n * m, "k");
    inf.readEoln();

    set<pair<int, int>> edges;

    for (int i = 0; i < k; i++) {
        int a = inf.readInt(1, n, "a_i");
        inf.readSpace();
        int b = inf.readInt(1, m, "b_i");
        inf.readEoln();

        ensuref(edges.count(make_pair(a, b)) == 0,
                "Graph can't contain multiple edges between a pair of vertices");
        edges.insert(make_pair(a, b));
    }

    inf.readEof();
}
