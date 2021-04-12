#include "testlib.h"

using namespace std;

int main(int argc, char * argv[])
{
    setName("example of checker with points_info");
    registerTestlibCmd(argc, argv);

    double pa = ouf.readDouble();
    double ja = ans.readDouble();
                           
    string points_info = "pa=" + to_string(pa) + ",ja=" + to_string(ja);
    quitpi(points_info, "d=" + to_string(abs(ja - pa)));
}
