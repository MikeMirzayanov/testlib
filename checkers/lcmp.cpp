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

bool compareWords(string a, string b)
{
    vector<string> va, vb;
    stringstream sa;
    
    sa << a;
    string cur;
    while (sa >> cur)
        va.push_back(cur);

    stringstream sb;
    sb << b;
    while (sb >> cur)
        vb.push_back(cur);

    return (va == vb);
}

int main(int argc, char * argv[])
{
    setName("compare files as sequence of tokens in lines");
    registerTestlibCmd(argc, argv);

    std::string strAnswer;

    int n = 0;
    while (!ans.eof()) 
    {
      std::string j = ans.readString();

      if (j == "" && ans.eof())
        break;
      
      std::string p = ouf.readString();
      strAnswer = p;

      n++;

      if (!compareWords(j, p))
        quitf(_wa, "%d%s lines differ - expected: '%s', found: '%s'", n, ending(n).c_str(), j.c_str(), p.c_str());
    }
    
    if (n == 1 && strAnswer.length() <= 128)
        quitf(_ok, "%s", strAnswer.c_str());
    
    quitf(_ok, "%d lines", n);
}
