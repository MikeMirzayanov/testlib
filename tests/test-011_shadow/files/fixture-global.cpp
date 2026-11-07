// A local variable hides a global variable of the program: must still be diagnosed after testlib.h.
#include "testlib.h"

int value = 0;

int main() {
    int value = 1;
    return value;
}
