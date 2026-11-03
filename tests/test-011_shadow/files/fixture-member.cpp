// A method parameter hides a data member: diagnosed by GCC and Clang. With MSVC, testlib.h
// disables C4458 for the rest of the translation unit, and that is not changed.
#include "testlib.h"

struct Counter {
    int count;
    void set(int count) {
        this->count = count;
    }
};

int main() {
    Counter counter;
    counter.set(0);
    return counter.count;
}
