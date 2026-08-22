#include "testlib.h"

#ifdef format
#error format must not be a macro
#endif

#ifdef println
#error println must not be a macro
#endif

#include <chrono>

#if defined(__has_include)
#if ((__cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)) && __has_include(<format>)
#include <format>
#endif
#if ((__cplusplus > 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)) && __has_include(<print>)
#include <print>
#endif
#endif

struct Object {
    int format() const { return 1; }
};

#if defined(__GNUC__) || defined(__clang__)
void attributed(const char *, ...) __attribute__((format(printf, 1, 2)));
#endif

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    if (argc > 1 && string(argv[1]) == "invalid")
        println(format(string("%"), 1));

    Object object;
    ensure(object.format() == 1);
    println("literal");
    println("two", 2);
    println("many", 1, 2, 3, 4, 5, 6, 7);
    char mutableText[] = "mutable";
    const char *pointerText = "pointer";
    string stringText = "string";
    println(mutableText, 4);
    println(pointerText, 5);
    println(stringText, 6);
    vector<int> values;
    values.push_back(7);
    values.push_back(8);
    println(values.begin(), values.end());
    const char *pointerFormat = "%d";
    string stringFormat = "%d";
    println(format("%d", 42));
    println(format(pointerFormat, 43));
    println(format(stringFormat, 44));
    println(format("[a-z]{10}%s", "x"));
    println("answer {}", 45);
#ifdef __cpp_lib_format
    ensure(std::format("{}", 46) == "46");
#endif
#ifdef __cpp_lib_print
    if (false) {
        std::println("{}", 47);
        println();
    }
#endif
}
