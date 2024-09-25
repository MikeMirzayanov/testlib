# Testlib

## Intro

This project contains a C++ implementation of testlib. It is already being used in many programming contests in Russia, such as the Russian National Olympiad in Informatics and different stages of ICPC. Join!

The library's C++ code is tested for compatibility with standard C++11 and higher on different versions of `g++`, `clang++`, and Microsoft Visual C++.

This code has been used many times in Codeforces contests.

## Samples

### Checker

This sample checker expects the same integer in the output and the answer. It ignores all white-spaces. See more examples in the package.

```c++
#include "testlib.h"

int main(int argc, char * argv[]) {
    setName("compares two signed integers");
    registerTestlibCmd(argc, argv);
    int ja = ans.readInt();
    int pa = ouf.readInt();
    if (ja != pa)
        quitf(_wa, "expected %d, found %d", ja, pa);
    quitf(_ok, "answer is %d", ja);
}
```

### Interactor

This sample interactor reads pairs of numbers from the input file, sends them to another program, reads
the result, and writes it to an output file (to be verified later). Another option could be to terminate
the interactor with `quitf(_wa, <comment>)`.

```c++
#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    setName("Interactor A+B");
    registerInteraction(argc, argv);

    // reads number of queries from test (input) file
    int n = inf.readInt();
    for (int i = 0; i < n; i++) {
        // reads query from test (input) file
        int a = inf.readInt();
        int b = inf.readInt();

        // writes query to the solution, endl makes flush
        cout << a << " " << b << endl;

        // writes output file to be verified by checker later
        tout << ouf.readInt() << endl;
    }

    // just message
    quitf(_ok, "%d queries processed", n);
}
```

### Validator

This code reads input from the standard input and checks that it contains only one integer between 1 and 100, inclusive. It also validates that the file ends with EOLN and EOF. On Windows, it expects #13#10 as EOLN, and it expects #10 as EOLN on other platforms. It does not ignore white-spaces, so it works very strictly. It will return a non-zero code in the case of illegal input and write a message to the standard output. See more examples in the package.

```c++
#include "testlib.h"

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);
    inf.readInt(1, 100, "n");
    inf.readEoln();
    inf.readEof();
}
```

### Generator

This generator outputs a random token to the standard output, containing Latin letters or digits. The length of the token will be between 1 and 1000, inclusive. It will use a uniformly distributed random generator. To generate different values, call it with different command-line parameters. It is typical behavior for a testlib generator to set up randseed by command line. See more examples in the package.

```c++
#include "testlib.h"

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    println(rnd.next(1, 10)); /* Random number in the range [1,10]. */
    println(rnd.next("[a-zA-Z0-9]{1,1000}")); /* Random word of length [1,1000]. */
}
```

This generator outputs a random permutation; the size is equal to the first command-line argument.

```c++
#include "testlib.h"

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    
    int n = opt<int>(1);
    println(n);
    println(rnd.perm(n, 1));
}
```
