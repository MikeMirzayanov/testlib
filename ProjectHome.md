## Intro ##

This project contains C++ implementation of the testlib. It already used on many programming contests in Russia (Russian National Olympiad in Informatics, different stages of ACM-ICPC). Join!

The library C++ code is tested for compatibility with Borland C++ (5.6), Microsoft Visual C++ (2003+), g++ (MinGW 4.0.2). Also we don't hear any issues about incorrect work with other compilers.

This code used many times on Open Team Programming Collegiate Cup contests (http://opencup.ru/) running on Linux platform and compiled by g++ compiler.

## Samples ##

### Checker ###
This is sample checker which expects in the output and in the answer the same integer. It ignores all white-spaces. See more examples in the package.
```
#include "testlib.h"

int main(int argc, char * argv[])
{
    setName("compares two signed integers");
    registerTestlibCmd(argc, argv);
    
    int ja = ans.readInt();
    int pa = ouf.readInt();
    
    if (ja != pa)
        quitf(_wa, "expected %d, found %d", ja, pa);
    
    quitf(_ok, "answer is %d", ja);
}
```

### Validator ###
This code reads input from the standard input and checks that it contains the only integer between 1 and 100, inclusive. Also validates that file ends with EOLN and EOF.
On Windows it expects #13#10 as EOLN and it expects #10 as EOLN on other platforms.
It doesn't ignore white-spaces, so it works very strict.
It will return non-zero code in case of illegal input and writes message into the standard output. See more examples in the package.
```
#include "testlib.h"

using namespace std;

int main()
{
    registerValidation();
    
    inf.readInt(1, 100);
    inf.readEoln();
    inf.readEof();

    return 0;
}
```

### Generator ###
This generator outputs into the standard output random token, containing Latin letters or digits. The length of the token will be between 1 and 1000, inclusive. It will use uniformly distributed random. To generate different values, call it with different command line parameters. It is typical behaviour of testlib generator to setup randseed by command line. See more examples in the package.
```
#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv);

    cout << rnd.next("[a-zA-Z0-9]{1,1000}") << endl;

    return 0;
}
```

