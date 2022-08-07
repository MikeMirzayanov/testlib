#include "testlib.h"

#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    int n = opt<int>(1);
    int t = opt<int>(2);

    vector<int> p(n);
    for (int i = 1; i < n; i++)
        p[i] = rnd.wnext(i, t);

    vector<int> perm = rnd.perm(n);

    vector<pair<int, int>> edges;
    for (int i = 1; i < n; i++)
        if (rnd.next(2))
            edges.push_back(make_pair(perm[i], perm[p[i]]));
        else
            edges.push_back(make_pair(perm[p[i]], perm[i]));

    shuffle(edges.begin(), edges.end());

    println(n);
    for (auto edge: edges)
        println(edge.first + 1, edge.second + 1);
}
