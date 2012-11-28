#include "testlib.h"

const string YES = "YES";
const string NO = "NO";

std::string upper(std::string s)
{
    for (size_t i = 0; i < s.length(); i++)
        if ('a' <= s[i] && s[i] <= 'z')
            s[i] = s[i] - 'a' + 'A';
    return s;
}

int main(int argc, char * argv[])
{
    setName(YES + " or " + NO + " (case insensetive)");
    registerTestlibCmd(argc, argv);

    std::string ja = upper(ans.readWord());
    std::string pa = upper(ouf.readWord());

    quitif(ja != YES && ja != NO, _fail, "%s or %s expected in answer, but %s found",
        YES.c_str(), NO.c_str(), __testlib_part(ja).c_str());

    quitif(pa != YES && pa != NO, _pe, "%s or %s expected, but %s found",
        YES.c_str(), NO.c_str(), __testlib_part(ja).c_str());

    quitif(ja != pa, _wa, "expected %s, found %s", __testlib_part(ja).c_str(), __testlib_part(pa).c_str());

    quitf(_ok, "answer is %s", ja.c_str());
}
