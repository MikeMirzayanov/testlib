#include "testlib.h"

using namespace std;

string ending(int x)
{
    x %= 100;
    if (x / 10 == 1)
        return "th";
    if (x % 10 == 1)
        return "st";
    if (x % 10 == 2)
        return "nd";
    if (x % 10 == 3)
        return "rd";
    return "th";
}

string part(const string& s)
{
    if (s.length() <= 64)
        return s;
    else
        return s.substr(0, 30) + "..." + s.substr(s.length() - 31, 31);
}

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
      if (j != p)
        quitf(_wa, "%d%s words differ - expected: '%s', found: '%s'", n, ending(n).c_str(), part(j).c_str(), part(p).c_str());
    }

    if (ans.seekEof() && ouf.seekEof())
    {
        if (n == 1)
            quitf(_ok, "%s", part(strAnswer).c_str());
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
