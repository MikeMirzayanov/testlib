#include "testlib.h"

#include <string>

using namespace std;

int main(int argc, char * argv[])
{
    setName("example of scored checker");
    registerTestlibCmd(argc, argv);

    double ja = ans.readDouble();
    double pa = ouf.readDouble();

    quitp(fabs(ja - pa), "ja=%.4lf pa=%.4lf", ja, pa);
}
