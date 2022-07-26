#include "testlib.h"

#include <vector>
#include <set>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    int n = opt<int>(1);
    int m = opt<int>(2);
    size_t k = opt<int>(3);

    int t = rnd.next(-2, 2);

    set<pair<int, int>> edges;

    while (edges.size() < k) {
        int a = rnd.wnext(n, t);
        int b = rnd.wnext(m, t);
        edges.insert(make_pair(a, b));
    }

    vector<pair<int, int>> e(edges.begin(), edges.end());
    shuffle(e.begin(), e.end());

    vector<int> pa = rnd.perm(n, 1);
    vector<int> pb = rnd.perm(m, 1);

    println(n, m, e.size());
    for (auto edge: e)
        println(pa[edge.first], pb[edge.second]);
}
