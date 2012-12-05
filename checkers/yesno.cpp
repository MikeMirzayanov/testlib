#include "testlib.h"
#include <string>

using namespace std;

const string YES = "YES";
const string NO = "NO";

int main(int argc, char * argv[])
{
    setName((YES + " or " + NO + " (case insensetive)").c_str());
    registerTestlibCmd(argc, argv);

    std::string ja = upperCase(ans.readWord());
    std::string pa = upperCase(ouf.readWord());

    quitif(ja != YES && ja != NO,
        _fail, "%s or %s expected in answer, but %s found",
        YES.c_str(), NO.c_str(), compress(ja).c_str());

    quitif(pa != YES && pa != NO,
        _pe, "%s or %s expected, but %s found",
        YES.c_str(), NO.c_str(), compress(ja).c_str());

    quitif(ja != pa,
        _wa, "expected %s, found %s",
        compress(ja).c_str(), compress(pa).c_str());

    quitf(_ok, "answer is %s", ja.c_str());
}
