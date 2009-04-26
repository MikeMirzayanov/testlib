#include "testlib.h"

std::string upper(std::string sa)
{
    for (size_t i = 0; i < sa.length(); i++)
        if ('a' <= sa[i] && sa[i] <= 'z')
            sa[i] = sa[i] - 'a' + 'A';
    return sa;
}

int main(int argc, char * argv[])
{
    setName("YES or NO (case insensetive)");
    registerTestlibCmd(argc, argv);

    std::string ja = upper(ans.readWord());
    std::string pa = upper(ouf.readWord());

    if (pa != "YES" && pa != "NO")
        quitf(_pe, "YES or NO expected, but %s found", pa.c_str());

    if (ja != "YES" && ja != "NO")
        quitf(_fail, "YES or NO expected in answer, but %s found", ja.c_str());

    if (ja != pa)
        quitf(_wa, "expected %s, found %s", ja.c_str(), pa.c_str());

    quitf(_ok, "answer is %s", ja.c_str());
}
