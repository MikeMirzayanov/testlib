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

int main()
{
    registerValidation();

    int n = inf.readInt(1, 400, "n");
    inf.readSpace();
    int m = inf.readInt(1, 400, "m");
    inf.readSpace();
    int k = inf.readInt(0, n * m, "k");
    inf.readEoln();

    set<pair<int,int> > edges;

    forn(i, k)
    {
        int a = inf.readInt(1, n, "a_i");
        inf.readSpace();
        int b = inf.readInt(1, m, "b_i");
        inf.readEoln();

        ensuref(edges.count(make_pair(a, b)) == 0, "Graph can't contain multiple edges between a pair of vertices");
        edges.insert(make_pair(a, b));
    }

    inf.readEof();
    return 0;
}
