#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); i++)

int main(int argc, char * argv[]) {
    registerGen(argc, argv, 1);
    int n = 1000000;
    printf("%d\n", n);
    forn(i, n) {
        printf("%.5f", 1000000.0 + i / 100000.00);
        if (i + 1 < n)
            printf(" ");
        else    
            printf("\n");
    }
}
