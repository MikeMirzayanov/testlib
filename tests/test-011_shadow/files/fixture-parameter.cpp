// A local variable hides a function parameter: must still be diagnosed after testlib.h.
#include "testlib.h"

int twice(int count) {
    {
        int count = 2;
        return count;
    }
}

int main() {
    return twice(0);
}
