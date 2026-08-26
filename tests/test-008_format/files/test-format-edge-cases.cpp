#include "testlib.h"

#if defined(__cpp_lib_format)
#define TEST_FORMAT_USES_TEMPLATE_BACKEND
#elif (defined(__cplusplus) && __cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#if defined(__has_include)
#if __has_include(<format>)
#define TEST_FORMAT_USES_TEMPLATE_BACKEND
#endif
#endif
#endif

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    ensure(format("a%cb", 0).size() == 1);
    ensure(format("%c", 0).size() == 0);

#if defined(__linux__)
    int width = 10;
    std::string decreasing = format("abc%n%*s", &width, width, "x");
#ifdef TEST_FORMAT_USES_TEMPLATE_BACKEND
    ensure(decreasing == "abc  x");
#else
    ensure(decreasing == "abc         x");
#endif
    ensure(width == 3);

    width = 2;
    std::string growing = format("%*sX%n", width, "", &width);
    ensure(growing.size() == 3);
#ifdef TEST_FORMAT_USES_TEMPLATE_BACKEND
    ensure(width == 4);
#else
    ensure(width == 3);
#endif
#endif

    println("format edge cases passed");
}

#undef TEST_FORMAT_USES_TEMPLATE_BACKEND
