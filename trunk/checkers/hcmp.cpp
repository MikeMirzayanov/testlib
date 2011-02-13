#include "testlib.h"

#include <string>

using namespace std;

bool isNumeric(string p)
{
    bool minus = false;

    if (p[0] == '-')
        minus = true,
        p = p.substr(1);

    for (int i = 0; i < p.length(); i++)
        if (p[i] < '0' || p[i] > '9')
            return false;

    if (minus)
        return (p.length() > 0 && (p.length() == 1 || p[0] != '0')) && (p.length() > 1 || p[0] != '0');
    else
        return p.length() > 0 && (p.length() == 1 || p[0] != '0');
}

int main(int argc, char * argv[])
{
    setName("compare two signed huge integers");
    registerTestlibCmd(argc, argv);
    
    string ja = ans.readWord();
    string pa = ouf.readWord();

    if (!isNumeric(ja))
        quitf(_fail, "%s is not valid integer", __testlib_part(ja).c_str());

    if (!ans.seekEof())
        quitf(_fail, "expected exactly one token in the answer file");
    
    if (!isNumeric(pa))
        quitf(_pe, "%s is not valid integer", __testlib_part(pa).c_str());

    if (ja != pa)
        quitf(_wa, "expected %s, found %s", __testlib_part(ja).c_str(), __testlib_part(pa).c_str());
    
    quitf(_ok, "answer is %s", __testlib_part(ja).c_str());
}
