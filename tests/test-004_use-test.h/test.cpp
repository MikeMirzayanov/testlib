#define TESTLIB_THROW_EXIT_EXCEPTION_INSTEAD_OF_EXIT

#include "testlib.h"
#include "test.h"

using namespace std;

#include "tests/test-join.cpp"
#include "tests/test-split.cpp"
#include "tests/test-tokenize.cpp"
#include "tests/test-opts.cpp"
#include "tests/test-instream.cpp"
#include "tests/test-pattern.cpp"
#include "tests/test-stringToLongLong.cpp"
#include "tests/test-stringToUnsignedLongLong.cpp"

int main() {
    disableFinalizeGuard();
    run_tests();
}
