/*
 * Outputs a string of the given structure.
 *
 * gs <number of parts k> <part1_repeat_count> <part1_period> <part2_repeat_count> <part2_period> ... <partk_repeat_count> <partk_period>
 *
 * Examples:
 *     gs 1 4 ab => abababab 
 *     gs 2 5 a 1 b => aaaaab
 *     gs 3 1 a 5 b 1 a => abbbbba
 */

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    string t;
    int n = opt<int>(1);
    for (int i = 2; i <= 1 + 2 * n; i += 2) {
        int k = opt<int>(i);
        string s = opt<string>(i + 1);
        for (int j = 0; j < k; j++)
            t += s;
    }
    println(t);
}
