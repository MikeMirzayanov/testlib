/**
 * Checker to compare output and answer in the form:
 *
 * Case 1: <token> <token> ... <token>
 * Case 2: <token> <token> ... <token>
 * ...
 * Case n: <token> <token> ... <token>
 *
 * Tokens in each case should not contain "Case"
 */

#include "testlib.h"
#include <vector>
#include <string>

using namespace std;

vector<string> readStreamCase(InStream &in, int testCase, bool &prereadCase) {
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

    vector<string> result;
    while (!in.seekEof()) {
        string token = in.readToken();
        if (token == "Case") {
            prereadCase = true;
            break;
        }

        result.push_back(token);
    }

    return result;
}

string stringsToString(const vector<string> &a) {
    if (a.empty())
        return "\"\" [size=0]";

    string elems;
    for (const auto& token: a)
        elems += token + " ";

    return format("\"%s\" [size=%u]", compress(trim(elems)).c_str(), (unsigned int) (a.size()));
}

int main(int argc, char *argv[]) {
    setName("Tokens checker with testcase-support");
    registerTestlibCmd(argc, argv);

    int testCase = 0;

    bool ansPrereadCase = false;
    bool oufPrereadCase = false;

    while (!ans.seekEof()) {
        testCase++;

        vector<string> ja = readStreamCase(ans, testCase, ansPrereadCase);
        vector<string> pa = readStreamCase(ouf, testCase, oufPrereadCase);

        if (ja != pa) {
            string js = stringsToString(ja);
            string ps = stringsToString(pa);
            quitf(_wa, "Sequences differ: jury has %s, but participant has %s [test case %d]", js.c_str(), ps.c_str(),
                  testCase);
        }
    }

    quitf(_ok, "%d test cases(s)", testCase);
}
