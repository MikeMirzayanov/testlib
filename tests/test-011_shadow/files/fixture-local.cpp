// A local variable hides another local variable: must still be diagnosed after testlib.h.
#include "testlib.h"

int main() {
    int count = 0;
    {
        int count = 1;
        (void)count;
    }
    return count;
}
