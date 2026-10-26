#include "testlib.h"

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    ensure(format("a%cb", 0).size() == 1);
    ensure(format("%c", 0).size() == 0);

    ensure(format("plain") == "plain");
    ensure(format(std::string("100%%")) == "100%");

#if defined(__linux__)
    int width = 10;
    std::string decreasing = format("abc%n%*s", &width, width, "x");
    ensure(decreasing == "abc         x");
    ensure(width == 3);

    width = 2;
    std::string growing = format("%*sX%n", width, "", &width);
    ensure(growing == "  X");
    ensure(width == 3);
#endif

    println("format edge cases passed");
}
