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

    int n = opt<int>(1);
    int t = opt<int>(2);

    vector<int> p(n);
    forn(i, n)
        if (i > 0)
            p[i] = rnd.wnext(i, t);

    println(n);
    vector<int> perm(n);
    forn(i, n)
        perm[i] = i;
    shuffle(perm.begin() + 1, perm.end());
    vector<pair<int,int> > edges;

    for (int i = 1; i < n; i++)
        if (rnd.next(2))
            edges.push_back(make_pair(perm[i], perm[p[i]]));
        else
            edges.push_back(make_pair(perm[p[i]], perm[i]));

    shuffle(edges.begin(), edges.end());

    for (int i = 0; i + 1 < n; i++)
        println(edges[i].first + 1, edges[i].second + 1);

    println(rnd.next("[a-z]{100}"));
    printf("%.04f\n", rnd.next(3.1415));

    {
        long long LL = 12345678901234LL;
        long long LR = 82345678901234LL;
        println(rnd.next(LL, LR));
    }

    {
        unsigned long long LL = 1345678901234ULL;
        unsigned long long LR = 8345678901234ULL;
        println(rnd.next(LL, LR));
    }

    {
        unsigned long LL = 134567UL;
        unsigned long LR = 834567UL;
        println(rnd.next(LL, LR));
    }

    {
        int LL = 134567;
        int LR = 834567;
        println(rnd.next(LL, LR));
    }

    vector<int> a;
    forn(i, 100)
        a.push_back(i * i);
    println(rnd.any(a));
    println(rnd.perm(10));
    println(rnd.perm(0));
    println(rnd.perm(0, 1));
    println(rnd.perm(10, 1));
    println(rnd.distinct(10, 15, 100));
    println(rnd.distinct(0, 15, 100));
    println(rnd.partition(10, 100, 3));
    println(rnd.partition(0, 0, 10));
    println(rnd.partition(1, 0, 0));
    println(rnd.wnext(1000, 2));
    println(rnd.wnext(1000LL, 2LL));
    println(rnd.wnext(1000, 20));
    println(rnd.wnext(1000LL, 20LL));
    println(rnd.wnext(100000, 200));
    println(rnd.wnext(100000LL, 200LL));
}
