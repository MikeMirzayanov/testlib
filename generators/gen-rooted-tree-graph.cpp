#include "testlib.h"

#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    int n = opt<int>(1);
    int t = opt<int>(2);

    vector<int> p(n);
    // p[i] is the parent of i-th vertex in 0-numeration without shuffling
    for (int i = 1; i < n; i++)
        p[i] = rnd.wnext(i, t);

    vector<int> perm(n);
    for (int i = 0; i < n; i++)
        perm[i] = i;
    shuffle(perm.begin() + 1, perm.end());

    vector<int> pp(n - 1);
    // pp[i] is the parent of (i+2)-nd vertex in 1-numeration after shuffling
    for (int i = 1; i < n; i++)
        pp[perm[i] - 1] = perm[p[i]] + 1;

    println(n);
    println(pp);
}
