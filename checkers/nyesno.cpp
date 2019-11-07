#include "testlib.h"
#include <string>

using namespace std;

const string YES = "YES";
const string NO = "NO";

int main(int argc, char * argv[])
{
    setName("%s", ("multiple " + YES + "/" + NO + " (case insensetive)").c_str());
    registerTestlibCmd(argc, argv);

    int index = 0, yesCount = 0, noCount = 0;
    std::string pa;
    while (!ans.seekEof() && !ouf.seekEof())
    {
        index++;
        std::string ja = upperCase(ans.readToken());
        pa = upperCase(ouf.readToken());

        if (ja != YES && ja != NO)
            quitf(_fail, "%s or %s expected in answer, but %s found [%d%s token]",
                YES.c_str(), NO.c_str(), compress(ja).c_str(), index, englishEnding(index).c_str());

        if (pa == YES)
            yesCount++;
        else if (pa == NO)
            noCount++;
        else
            quitf(_pe, "%s or %s expected, but %s found [%d%s token]",
                YES.c_str(), NO.c_str(), compress(pa).c_str(), index, englishEnding(index).c_str());

        if (ja != pa)
            quitf(_wa, "expected %s, found %s [%d%s token]",
                compress(ja).c_str(), compress(pa).c_str(), index, englishEnding(index).c_str());
    }

    int extraInAnsCount = 0;
    while (!ans.seekEof())
    {
        ans.readToken();
        extraInAnsCount++;
    }
    
    int extraInOufCount = 0;
    while (!ouf.seekEof())
    {
        ouf.readToken();
        extraInOufCount++;
    }

    if (extraInAnsCount > 0)
        quitf(_wa, "Answer contains longer sequence [length = %d], but output contains %d elements", index + extraInAnsCount, index);
    
    if (extraInOufCount > 0)
        quitf(_wa, "Output contains longer sequence [length = %d], but answer contains %d elements", index + extraInOufCount, index);

    if (index == 0)
        quitf(_ok, "Empty output");
    else if (index == 1)
        quitf(_ok, "%s", pa.c_str());
    else
        quitf(_ok, "%d token(s): yes count is %d, no count is %d", index, yesCount, noCount);

    quitf(_fail, "Impossible case");
}
