#include "testlib.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

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
        quitif(j != p,
            _wa, "%d%s lines differ - expected: '%s', found: '%s'",
            n, englishEnding(n).c_str(), compress(j).c_str(), compress(p).c_str());
    }
    
    quitif(n == 1 && strAnswer.length() <= 128,
        _ok, "%s", strAnswer.c_str());
    
    quitf(_ok, "%d lines", n);
}
