// Globals of a program, declared after testlib.h, then the templates of testlib.h are instantiated.
// MSVC checks a template body where it is instantiated, and Clang with delayed template parsing (the
// default for the MSVC target before C++20) checks the parameters there as well, so a local or a
// parameter of a testlib template named like one of these globals would be reported in testlib.h.
// The list holds the names those locals and parameters had before they got the "_" prefix, and names
// that programs commonly use. ("index" is left out: glibc declares a function of that name.)
#include "testlib.h"
#include <list>
#include <set>

int about, ap, buffer, current, d, digit, expected, expectedString, first, foundString, i, it, message,
    n, n_, negative, p, pos, preparedVariableName, prepend, repeated, result, result_sum, s, septums, sign,
    size, ss, sum_, t_string, trailingZeroCount, vals, value, written, x;
int a, args, b, begin, c, collection, default_value, end, fmt, found, from, key, last, lower, min_part,
    nonnegative, points, prependFormat, s_, separator, t, to, type, upper, variableName;
int e, f, g, h, j, k, l, m, q, r, u, v, w, y, z;
int N, M, K, INF, MOD, MAXN, cnt, len, res, sum, total, answer, mx, mn, dp, cur, ok, used, adj, edges, tmp,
    lo, hi, mid, mod, count, rest, text;

#include "shadow-probe.cpp"
