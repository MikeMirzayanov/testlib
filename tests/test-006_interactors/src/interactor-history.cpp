#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    setName("Interactor A+B with history check");
    registerInteraction(argc, argv);

    // reads number of queries from test (input) file
    int n = inf.readInt();
    for (int i = 0; i < n; i++) {
        // reads query from test (input) file
        int a = inf.readInt();
        int b = inf.readInt();

        // writes query to the solution, endl makes flush
        cout << a << " " << b << endl;

        char first = ouf.curChar();
        char next = ouf.nextChar();
        ensure(first == next);
        ouf.unreadChar(next);
        ensure(ouf.reader->getReadChars().empty());

        // writes output file to be verified by checker later
        tout << ouf.readInt() << endl;
        ensure(ouf.reader->getReadChars().empty());
    }

    ensure(!ouf.eof());
    ensure(ouf.reader->getReadChars().empty());

    // just message
    quitf(_ok, "%d queries processed", n);
}
