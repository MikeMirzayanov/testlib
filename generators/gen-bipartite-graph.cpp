#include "testlib.h"

#include <vector>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int n = opt<int>(1);
    int m = opt<int>(2);
    size_t k = opt<int>(3);

    int t = rnd.next(-2, 2);

    set<pair<int,int> > edges;

    while (edges.size() < k)
    {
        int a = rnd.wnext(n, t);
        int b = rnd.wnext(m, t);
        edges.insert(make_pair(a, b));
    }

    vector<pair<int,int> > e(edges.begin(), edges.end());
    shuffle(e.begin(), e.end());

    vector<int> pa(n);
    for (int i = 0; i < n; i++)
        pa[i] = i + 1;
    shuffle(pa.begin(), pa.end());

    vector<int> pb(m);
    for (int i = 0; i < m; i++)
        pb[i] = i + 1;
    shuffle(pb.begin(), pb.end());

    println(n, m, e.size());
    for (auto [a, b] : e)
        println(pa[a], pb[b]);

    return 0;
}
