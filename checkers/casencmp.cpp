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
#include <vector>
#include <string>

using namespace std;

vector<long long> readStreamCase(InStream &in, int testCase, bool &prereadCase) {
    if (!prereadCase) {
        string caseStr = in.readToken();
        if (caseStr != "Case")
            in.quitf(_pe, "Expected 'Case' but found '%s' [test case %d]", compress(caseStr).c_str(), testCase);
    }

    string numExpStr = to_string(testCase) + ":";
    string numStr = in.readToken();
    if (numExpStr != numStr)
        in.quitf(_pe, "Expected '%s' but found '%s' [test case %d]", compress(numExpStr).c_str(),
                 compress(numStr).c_str(), testCase);

    vector<long long> result;
    while (!in.seekEof()) {
        string token = in.readToken();
        if (token == "Case") {
            prereadCase = true;
            break;
        }

        result.push_back(stringToLongLong(in, token.c_str()));
    }

    return result;
}

string longLongsToString(const vector<long long> &a) {
    if (a.empty())
        return "\"\" [size=0]";

    string elems;
    if (a.size() <= 5) {
        for (auto elem: a)
            elems += vtos(elem) + " ";
    } else {
        for (int i = 0; i < 3; i++)
            elems += vtos(a[i]) + " ";
        elems += "... ";
        for (int i = 0; i < 2; i++)
            elems += vtos(a[a.size() - 2 + i]) + " ";
    }

    return format("\"%s\" [size=%u]", trim(elems).c_str(), (unsigned int) (a.size()));
}

int main(int argc, char *argv[]) {
    setName("Many int64s checker with testcase-support");
    registerTestlibCmd(argc, argv);

    int testCase = 0;

    bool ansPrereadCase = false;
    bool oufPrereadCase = false;

    while (!ans.seekEof()) {
        testCase++;

        vector<long long> ja = readStreamCase(ans, testCase, ansPrereadCase);
        vector<long long> pa = readStreamCase(ouf, testCase, oufPrereadCase);

        if (ja != pa) {
            string js = longLongsToString(ja);
            string ps = longLongsToString(pa);
            quitf(_wa, "Sequences differ: jury has %s, but participant has %s [test case %d]", js.c_str(), ps.c_str(),
                  testCase);
        }
    }

    quitf(_ok, "%d test cases(s)", testCase);
}
