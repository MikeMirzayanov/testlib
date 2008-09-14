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

int main(int argc, char * argv[])
{
    setName("compare sequences of tokens");
    registerTestlibCmd(argc, argv);

    std::string strAnswer;

    int n = 0;

    while (!ans.seekEof()) 
    {
      n++;
      std::string j = ans.readWord();
      std::string p = ouf.readWord();
      strAnswer = p;
      if (j != p)
        quitf(_wa, "%d%s words differ - expected: '%s', found: '%s'", n, ending(n).c_str(), j.c_str(), p.c_str());
    }

    if (n == 1 && strAnswer.length() <= 128)
        quitf(_ok, "%s", strAnswer.c_str());

    quitf(_ok, "%d words", n);
}
