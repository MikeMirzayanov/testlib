#include "testlib.h"

using namespace std;

int main(int argc, char * argv[])
{
    registerTestlibCmd(argc, argv);

    int z = 0;
    while (!inf.eof())
    {
    	z++;

    	if (z % 4 == 0)
    		printf("%d\n", inf.readChar());
    		
    	if (z % 4 == 1)
    		printf("%d\n", inf.curChar());
    		
    	if (z % 4 == 2)
    		printf("%s\n", inf.readLine().c_str());
    		
    	if (z % 4 == 3)
    		printf("%s\n", inf.readToken().c_str());
    }

    while (!ouf.seekEof())
    {
    	z++;

    	if (z % 4 == 3)
    		printf("%d\n", ouf.readChar());
    		
    	if (z % 4 == 1)
    		printf("%d\n", ouf.curChar());
    		
    	if (z % 4 == 0)
    		printf("%s\n", ouf.readLine().c_str());
    		
    	if (z % 4 == 2)
    		printf("%s\n", ouf.readToken().c_str());
    }

    printf("Done\n");

    quitf(_ok, "ok");
}
