#include "testlib.h"

using namespace std;

int main(int argc, char * argv[])
{
    setName("compare sequences of tokens");
    registerTestlibCmd(argc, argv);

    std::string strAnswer;

    int n = 0;

    while (!ans.seekEof() && !ouf.seekEof()) 
    {
        n++;
        std::string j = ans.readWord();
        std::string p = ouf.readWord();
        strAnswer = p;
        quitif(j != p,
            _wa, "%d%s words differ - expected: '%s', found: '%s'",
            n, englishEnding(n).c_str(), compress(j).c_str(), compress(p).c_str());
    }

    if (ans.seekEof() && ouf.seekEof())
    {
        if (n == 1)
            quitf(_ok, "%s", compress(strAnswer).c_str());
        else
            quitf(_ok, "%d words", n);
    }
    else
    {
        if (ans.seekEof())
            quitf(_wa, "Participant output contains extra tokens");
        else
            quitf(_wa, "Unexpected EOF in the participants output");
    }
}
