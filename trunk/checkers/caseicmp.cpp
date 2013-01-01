/**
 * Checker to compare output and answer in the form:
 *
 * Case 1: <number>
 * Case 2: <number>
 * ...
 * Case n: <number>
 *
 */

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

using namespace std;

#define forn(i, n) for (int i = 0; i < int(n); i++)

vector<long long> readStream(InStream& in, TResult pe)
{
    vector<long long> result;

    for (int testCase = 1; !in.seekEof(); testCase++)
    {
        string caseStr = in.readToken();
        if (caseStr != "Case")
            quitf(pe, "Expected 'Case' but found '%s' [test case %d]", compress(caseStr).c_str(), testCase);

        string numExpStr;
        stringstream ss;
        ss << testCase;
        ss >> numExpStr;
        numExpStr += ":";
        string numStr = in.readToken();
        if (numExpStr != numStr)
            quitf(pe, "Expected '%s' but found '%s' [test case %d]", compress(numExpStr).c_str(), compress(numStr).c_str(), testCase);

        result.push_back(in.readLong());
    }

    return result;
}

int main(int argc, char* argv[])
{
    setName("Single int64 checker with testcase-support");
    registerTestlibCmd(argc, argv);

    vector<long long> ja = readStream(ans, _fail);
    vector<long long> pa = readStream(ouf, _pe);

    forn(i, min(ja.size(), pa.size()))
        if (ja[i] != pa[i])
            quitf(_wa, "Expected %s found %s [test case %d]", vtos(ja[i]).c_str(), vtos(pa[i]).c_str(), i + 1);

    if (ja.size() != pa.size())
        quitf(_pe, "Expected %d test case(s) but found %d", ja.size(), pa.size());

    string message = format("%d case(s):", ja.size());
    if (ja.size() <= 5)
    {
        forn(i, ja.size())
            message += " " + vtos(ja[i]);
    }
    else
    {
        forn(i, 3)
            message += " " + vtos(ja[i]);
        message += " ...";
        forn(i, 2)
            message += " " + vtos(ja[ja.size() - 2 + i]);
    }

    quitf(_ok, "%s", message.c_str());
}
