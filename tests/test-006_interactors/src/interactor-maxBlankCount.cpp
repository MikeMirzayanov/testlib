#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    setName("Interactor maxBlankCount test");
    registerInteraction(argc, argv);

    // Set a low limit for testing (to verify it triggers)
    ouf.maxBlankCount = 1000;  // ouf is the stream from participant

    // Read the number of queries from input file
    int n = inf.readInt();

    for (int i = 0; i < n; i++) {
        // Send query to participant
        int query = inf.readInt();
        cout << query << endl;

        // Try to read response from participant
        // If participant sent > 1000 spaces, this will cause _pe
        int response = ouf.readInt();

        // Write result
        tout << response << endl;
    }

    quitf(_ok, "%d queries processed", n);
}
