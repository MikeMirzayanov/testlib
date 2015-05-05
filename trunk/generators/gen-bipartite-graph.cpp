#include "testlib.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <climits>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <bitset>
#include <utility>
#include <algorithm>

#define forn(i, n) for (int i = 0; i < int(n); i++)

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    size_t k = atoi(argv[3]);

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

    cout << n << " " << m << " " << e.size() << endl;

    forn(i, e.size())
        cout << pa[e[i].first] << " " << pb[e[i].second] << endl;

    return 0;
}
