#include "testlib.h"

#include <string>

using namespace std;

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);

    string mode = argc > 1 ? argv[1] : "";

    if (mode == "braces") {
        println(format("answer %d {}", 42));
    } else if (mode == "suppress") {
        suppressStdFormatSyntaxCheck();
        println(format("answer %d {}", 42));
    } else {
        ensure(false);
    }
}
