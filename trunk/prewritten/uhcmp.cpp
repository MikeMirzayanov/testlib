#include "testlib.h"

#include <string>

using namespace std;

bool isNumeric(string p)
{
    for (int i = 0; i < p.length(); i++)
        if (p[i] < '0' || p[i] > '9')
            return false;

    return p.length() > 0 && (p.length() == 1 || p[0] != '0');
}

int main(int argc, char * argv[])
{
    setName("compare two unsigned huge integers");
    registerTestlibCmd(argc, argv);
    
    string ja = ans.readWord();
    string pa = ouf.readWord();

    if (!isNumeric(ja))
        quitf(_fail, "%s is not numeric", ja.c_str());

    if (!isNumeric(pa))
        quitf(_pe, "%s is not numeric", pa.c_str());
    
    if (ja != pa)
        quitf(_wa, "expected %s, found %s", ja.c_str(), pa.c_str());
    
    quitf(_ok, "answer is %s", ja.c_str());
}
