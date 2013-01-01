#include "testlib.h"
#include <vector>

using namespace std;

int main(int argc, char * argv[])
{
    setName("compare unordered sequences of signed int%d numbers", 8 * sizeof(long long));

    registerTestlibCmd(argc, argv);

    vector<int> ja, pa;

    while (!ans.seekEof())
        ja.push_back(ans.readLong());

    while (!ouf.seekEof())
        pa.push_back(ouf.readLong());

    if (ja.size() != pa.size())
        quitf(_wa, "Expected %d elements, but %d found", ja.size(), pa.size());

    sort(ja.begin(), ja.end());
    sort(pa.begin(), pa.end());

    if (ja != pa)
        quitf(_wa, "Expected sequence and output are different (as unordered sequences) [size=%d]", ja.size());

    string message;
    
    if (ja.size() != 1)
        if (ja.empty())
            message = "empty sequence";
        else
            message = vtos(ja.size()) + " numbers (in increasing order):";
    else
        message = vtos(ja.size()) + " number:";
    
    if (ja.size() <= 5)
        for (int i = 0; i < min(int(ja.size()), 5); i++)
            message += " " + vtos(ja[i]);
    else
    {
        for (int i = 0; i < 2; i++)
            message += " " + vtos(ja[i]);
        message += " ...";
        for (int i = 0; i < 2; i++)
            message += " " + vtos(ja[ja.size() - 2 + i]);
    }
        
    quitf(_ok, "%s", message.c_str());
}
