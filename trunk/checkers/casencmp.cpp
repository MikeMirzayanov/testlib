/**
 * Checker to compare output and answer in the form:
 *
 * Case 1: <number> <number> <number> ... <number>
 * Case 2: <number> <number> <number> ... <number>
 * ...
 * Case n: <number> <number> <number> ... <number>
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

string token;

vector<long long> readStreamCase(InStream& in, TResult pe, int testCase, bool& prereadCase)
{
    if (!prereadCase)
    {
        string caseStr = in.readToken();
        if (caseStr != "Case")
            quitf(pe, "Expected 'Case' but found '%s' [test case %d]", compress(caseStr).c_str(), testCase);
    }

    string numExpStr;
    stringstream ss;
    ss << testCase;
    ss >> numExpStr;
    numExpStr += ":";
    string numStr = in.readToken();
    if (numExpStr != numStr)
        quitf(pe, "Expected '%s' but found '%s' [test case %d]", compress(numExpStr).c_str(), compress(numStr).c_str(), testCase);

    vector<long long> result;
    while (!in.seekEof())
    {
        in.readTokenTo(token);
        if (token == "Case")
        {
            prereadCase = true;
            break;
        }

        result.push_back(stringToLongLong(in, token.c_str()));
    }

    return result;
}

string longLongsToString(const vector<long long>& a)
{
    if (a.empty())
        return "\"\" [size=0]";
    
    string elems;
    if (a.size() <= 5)
    {
        forn(i, a.size())
            elems += vtos(a[i]) + " ";
    }
    else
    {
        forn(i, 3)
            elems += vtos(a[i]) + " ";
        elems += "... ";
        forn(i, 2)
            elems += vtos(a[a.size() - 2 + i]) + " ";
    }

    return format("\"%s\" [size=%d]", trim(elems).c_str(), a.size());
}

int main(int argc, char* argv[])
{
    setName("Many int64s checker with testcase-support");
    registerTestlibCmd(argc, argv);

    int testCase = 0;

    bool ansPrereadCase = false;
    bool oufPrereadCase = false;

    while (!ans.seekEof())
    {
        testCase++;

        vector<long long> ja = readStreamCase(ans, _fail, testCase, ansPrereadCase);
        vector<long long> pa = readStreamCase(ouf, _pe, testCase, oufPrereadCase);

        if (ja != pa)
        {
            string js = longLongsToString(ja);
            string ps = longLongsToString(pa);
            quitf(_wa, "Sequences differ: jury has %s, but participant has %s [test case %d]", js.c_str(), ps.c_str(), testCase);
        }
    }

    quitf(_ok, "%d test cases(s)", testCase);
}
