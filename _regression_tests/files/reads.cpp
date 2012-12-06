#include "testlib.h"

using namespace std;

int main(int argc, char * argv[])
{
    registerTestlibCmd(argc, argv);

    while (!inf.eof())
        printf("%d\n", inf.readChar());

    while (!ouf.seekEof())
        printf("%s\n", ouf.readToken().c_str());

    while (!ans.eof())
        printf("%s\n", ans.readLine().c_str());

    printf("Done\n");

    quitf(_ok, "ok");
}
