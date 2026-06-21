#include "testlib.h"

#include <string>

using namespace std;

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);

    string mode = argc > 1 ? argv[1] : "";

    if (mode == "braces") {
        println("answer {}", 42);
    } else if (mode == "indexed") {
        println("answer {0}", 42);
    } else if (mode == "spec") {
        println("answer {:04}", 42);
    } else if (mode == "escaped") {
        println("answer {{}}", 42);
    } else if (mode == "no-args") {
        println("answer {}");
    } else if (mode == "suppress") {
        suppressStdFormatSyntaxCheck();
        println("answer {}", 42);
    } else {
        ensure(false);
    }
}
