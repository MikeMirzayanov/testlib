// Globals of a program, declared after testlib.h, then the templates of testlib.h are instantiated.
// MSVC checks a template body where it is instantiated, so a local of a testlib template named like
// one of these globals would report C4459 in testlib.h. The list holds the names the locals of the
// templates had before they got the "_" prefix, and names that programs commonly use.
#include "testlib.h"
#include <list>
#include <set>

int about, ap, buffer, current, d, digit, expected, expectedString, first, foundString, i, it, message,
    n, n_, negative, p, pos, preparedVariableName, prepend, repeated, result, result_sum, s, septums, sign,
    size, ss, sum_, t_string, trailingZeroCount, vals, value, written, x;
int a, b, c, e, f, g, h, j, k, l, m, q, r, t, u, v, w, y, z;
int N, M, K, INF, MOD, MAXN, cnt, len, res, sum, total, answer, mx, mn, dp, cur, ok, used, adj, edges, tmp,
    lo, hi, mid, mod, count;

#include "shadow-probe.cpp"
