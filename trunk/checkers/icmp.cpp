#include "testlib.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
    setName("compare two signed int%d's", 8 * sizeof(int));
    registerTestlibCmd(argc, argv);
    
    int ja = ans.readInt();
    int pa = ouf.readInt();
    
    quitif(ja != pa, _wa, "expected %d, found %d", ja, pa);
    quitf(_ok, "answer is %d", ja);
}
