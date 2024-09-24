#include "testlib.h"

#if (__cplusplus >= 202002L && __has_include(<format>))
#   include <format>
#endif

using namespace std;

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    
    println(format("%d", 42));
    println(format("hello, %s!", "hat"));
    println(format("%s%d!", "'hat'", 42));
    println(format("%s%d!", "'%s'", 42));

    ensure(format("%f", 42.5).substr(0, 4) == "42.5");
}
