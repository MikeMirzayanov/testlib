#include "testlib.h"
#include <string>
#include <vector>
#include <sstream>

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
    setName("compare files as sequence of lines");
    registerTestlibCmd(argc, argv);

    std::string strAnswer;

    int n = 0;
    while (!ans.eof()) 
    {
      std::string j = ans.readString();

      if (j == "" && ans.eof())
        break;

      strAnswer = j;
      std::string p = ouf.readString();

      n++;
      quitif(j != p, _wa, "%d%s lines differ - expected: '%s', found: '%s'",
        n, ending(n).c_str(), __testlib_part(j).c_str(), __testlib_part(p).c_str());
    }
    
    quitif(n == 1 && strAnswer.length() <= 128, _ok, "%s", strAnswer.c_str());
    quitf(_ok, "%d lines", n);
}
