#include "testlib.h"

#include <vector>
#include <set>

using namespace std;

int leader(vector<int> &dsu, int idx) {
    return dsu[idx] == idx ? dsu[idx] : (dsu[idx] = leader(dsu, dsu[idx]));
}

bool merge(vector<int> &dsu, int a, int b) {
    a = leader(dsu, a);
    b = leader(dsu, b);
    if (a == b)
        return false;
    else {
        if (rnd.next(2) == 0)
            dsu[a] = b;
        else
            dsu[b] = a;
        return true;
    }
}

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(2, 100000, "n");
    inf.readEoln();

    vector<int> dsu(n);
    for (int i = 0; i < n; i++)
        dsu[i] = i;

    set<pair<int, int>> edges;

    for (int i = 0; i < n - 1; i++) {
        int x = inf.readInt(1, n, "x_i");
        inf.readSpace();
        int y = inf.readInt(1, n, "y_i");
        inf.readEoln();

        ensuref(x != y, "Tree can't contain loops");
        ensuref(edges.count(make_pair(x, y)) == 0, "Tree can't contain multiple edges between a pair of vertices");

        edges.insert(make_pair(x, y));
        edges.insert(make_pair(y, x));

        ensuref(merge(dsu, x - 1, y - 1), "Tree can't contain cycles");
    }

    inf.readEof();
}
