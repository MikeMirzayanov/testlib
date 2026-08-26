#include "testlib.h"

struct Flags {
    unsigned value : 6;
};

struct StreamableValue {
    int value;
};

std::ostream &operator<<(std::ostream &out, const StreamableValue &value) {
    return out << "s" << value.value;
}

#if __cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)
#define TEST_PRINTLN_REQUIRES_BITFIELD_CAST
#endif

#ifdef TEST_PRINTLN_DIRECT_BITFIELD_NEGATIVE
#ifndef TEST_PRINTLN_REQUIRES_BITFIELD_CAST
#error TEST_PRINTLN_NEGATIVE_NOT_APPLICABLE
#endif
#define TEST_PRINTLN_FIELD(value) (value)
#elif defined(TEST_PRINTLN_REQUIRES_BITFIELD_CAST)
#define TEST_PRINTLN_FIELD(value) (+(value))
#else
#define TEST_PRINTLN_FIELD(value) (value)
#endif

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    Flags flags = {13};
    StreamableValue streamableValue = {5};

    println("p2", TEST_PRINTLN_FIELD(flags.value));
    println("p3", 1, TEST_PRINTLN_FIELD(flags.value));
    println("p4", streamableValue, 2, TEST_PRINTLN_FIELD(flags.value));
    println("p5", 1, 2, 3, TEST_PRINTLN_FIELD(flags.value));
    println("p6", 1, 2, 3, 4, TEST_PRINTLN_FIELD(flags.value));
    println("p7", 1, 2, 3, 4, 5, TEST_PRINTLN_FIELD(flags.value));
    println("p8", streamableValue, 1, 2, 3, 4, 5, TEST_PRINTLN_FIELD(flags.value));
    println(1, 2, TEST_PRINTLN_FIELD(flags.value));
    println("mixed", streamableValue, TEST_PRINTLN_FIELD(flags.value));
}

#undef TEST_PRINTLN_FIELD
#undef TEST_PRINTLN_REQUIRES_BITFIELD_CAST
