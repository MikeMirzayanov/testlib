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

    std::cout << std::fixed << std::setprecision(4) << rnd.next((float) 42.0, (float) 2011.1109) << std::endl;
    std::cout << std::fixed << std::setprecision(4) << rnd.next((double) 42.0, (double) 2011.1109) << std::endl;
    std::cout << std::fixed << std::setprecision(4) << rnd.wnext((float) 42.0, (float) 2011.1109, 5) << std::endl;
    std::cout << std::fixed << std::setprecision(4) << rnd.wnext((double) 42.0, (double) 2011.1109, 5) << std::endl;
    std::cout << std::fixed << std::setprecision(4) << rnd.wnext((float) 42.0, (float) 2011.1109, -7) << std::endl;
    std::cout << std::fixed << std::setprecision(4) << rnd.wnext((double) 42.0, (double) 2011.1109, -7) << std::endl;

    std::cout << rnd.next((unsigned long long) 42, (unsigned long long) 2011) << std::endl;
    std::cout << rnd.next((unsigned int) 42, (unsigned int) 2011) << std::endl;
    std::cout << rnd.next((unsigned short) 42, (unsigned short) 2011) << std::endl;

    std::cout << rnd.wnext((unsigned long long) 42, (unsigned long long) 2011, -3) << std::endl;
    std::cout << rnd.wnext((unsigned int) 42, (unsigned int) 2011, -3) << std::endl;
    std::cout << rnd.wnext((unsigned short) 42, (unsigned short) 2011, -3) << std::endl;
    std::cout << rnd.wnext((unsigned long long) 42, (unsigned long long) 2011, 3) << std::endl;
    std::cout << rnd.wnext((unsigned int) 42, (unsigned int) 2011, 3) << std::endl;
    std::cout << rnd.wnext((unsigned short) 42, (unsigned short) 2011, 3) << std::endl;

    std::cout << rnd.wnext((signed long long) 42, (signed long long) 2011, -5) << std::endl;
    std::cout << rnd.wnext((signed int) 42, (signed int) 2011, -5) << std::endl;
    std::cout << rnd.wnext((signed short) 42, (signed short) 2011, -5) << std::endl;
    std::cout << rnd.wnext((signed long long) 42, (signed long long) 2011, 4) << std::endl;
    std::cout << rnd.wnext((signed int) 42, (signed int) 2011, 4) << std::endl;
    std::cout << rnd.wnext((signed short) 42, (signed short) 2011, 4) << std::endl;

    println(rnd.wany(a, 1));
    println(rnd.wany(a, -1));

    set<string> b;
    b.insert("a"); b.insert("b"); b.insert("c"); b.insert("d"); b.insert("e"); b.insert("f"); b.insert("g");
    println(rnd.any(b));

    {
        std::set<std::string> string_set;
        string_set.insert("a");
        string_set.insert("b");
        string_set.insert("c");
        string_set.insert("d");
        string_set.insert("e");
        string_set.insert("f");

        std::cout << rnd.any(string_set) << std::endl;
        std::cout << rnd.any(string_set.begin(), string_set.end()) << std::endl;
        std::cout << rnd.wany(string_set, 1) << std::endl;
        std::cout << rnd.wany(string_set.begin(), string_set.end(), 1) << std::endl;
        std::cout << rnd.wany(string_set, -1) << std::endl;
        std::cout << rnd.wany(string_set.begin(), string_set.end(), -1) << std::endl;
    }

    {
        std::multiset<std::string> string_multiset;
        string_multiset.insert("a");
        string_multiset.insert("b"); string_multiset.insert("b");
        string_multiset.insert("c"); string_multiset.insert("c"); string_multiset.insert("c");
        string_multiset.insert("d");
        string_multiset.insert("e"); string_multiset.insert("e"); string_multiset.insert("e");
        string_multiset.insert("f"); string_multiset.insert("f"); string_multiset.insert("f"); string_multiset.insert("f");

        std::cout << rnd.any(string_multiset) << std::endl;
        std::cout << rnd.any(string_multiset.begin(), string_multiset.end()) << std::endl;
        std::cout << rnd.wany(string_multiset, 2) << std::endl;
        std::cout << rnd.wany(string_multiset.begin(), string_multiset.end(), 1) << std::endl;
        std::cout << rnd.wany(string_multiset, -1) << std::endl;
        std::cout << rnd.wany(string_multiset.begin(), string_multiset.end(), -2) << std::endl;
    }
}
