#include "testlib.h"

#include <string>

using namespace std;

pattern pnum("0|-?[1-9][0-9]*");

bool isNumeric(const string& p)
{
    return pnum.matches(p);
}

int main(int argc, char * argv[])
{
    setName("compare two signed huge integers");
    registerTestlibCmd(argc, argv);
    
    string ja = ans.readWord();
    string pa = ouf.readWord();

    quitif(!isNumeric(ja), _fail, "%s is not valid integer", __testlib_part(ja).c_str());
    quitif(!ans.seekEof(), _fail, "expected exactly one token in the answer file");
    
    quitif(!isNumeric(pa), _pe, "%s is not valid integer", __testlib_part(pa).c_str());
    quitif(ja != pa, _wa, "expected %s, found %s", __testlib_part(ja).c_str(), __testlib_part(pa).c_str());
    
    quitf(_ok, "answer is %s", __testlib_part(ja).c_str());
}
