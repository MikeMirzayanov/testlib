// A constructor parameter hides a data member: diagnosed by GCC and Clang -Wshadow-all,
// MSVC does not diagnose it.
#include "testlib.h"

struct Counter {
    int count;
    explicit Counter(int count) : count(count) {}
};

int main() {
    Counter counter(0);
    return counter.count;
}
