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
#include <vector>
#include <string>

using namespace std;

vector<long long> readStream(InStream &in) {
    vector<long long> result;

    for (int testCase = 1; !in.seekEof(); testCase++) {
        string caseStr = in.readToken();
        if (caseStr != "Case")
            in.quitf(_pe, "Expected 'Case' but found '%s' [test case %d]", compress(caseStr).c_str(), testCase);

        string numExpStr = to_string(testCase) + ":";
        string numStr = in.readToken();
        if (numExpStr != numStr)
            in.quitf(_pe, "Expected '%s' but found '%s' [test case %d]", compress(numExpStr).c_str(),
                     compress(numStr).c_str(), testCase);

        result.push_back(in.readLong());
    }

    return result;
}

int main(int argc, char *argv[]) {
    setName("Single int64 checker with testcase-support");
    registerTestlibCmd(argc, argv);

    vector<long long> ja = readStream(ans);
    vector<long long> pa = readStream(ouf);

    for (unsigned int i = 0; i < min(ja.size(), pa.size()); i++)
        if (ja[i] != pa[i])
            quitf(_wa, "Expected %s found %s [test case %d]", vtos(ja[i]).c_str(), vtos(pa[i]).c_str(), i + 1);

    if (ja.size() != pa.size())
        quitf(_pe, "Expected %u test case(s) but found %u", (unsigned int) (ja.size()), (unsigned int) (pa.size()));

    string message = format("%u case(s):", (unsigned int) (ja.size()));
    if (ja.size() <= 5) {
        for (auto elem: ja)
            message += " " + vtos(elem);
    } else {
        for (int i = 0; i < 3; i++)
            message += " " + vtos(ja[i]);
        message += " ...";
        for (int i = 0; i < 2; i++)
            message += " " + vtos(ja[ja.size() - 2 + i]);
    }

    quitf(_ok, "%s", message.c_str());
}
