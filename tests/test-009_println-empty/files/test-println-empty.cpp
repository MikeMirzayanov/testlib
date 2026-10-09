#include "testlib.h"
#include <sstream>

using namespace std;

template<typename F>
string captured(F f) {
    ostringstream out;
    streambuf *old = cout.rdbuf(out.rdbuf());
    f();
    cout.rdbuf(old);
    return out.str();
}

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);

    vector<int> empty;
    vector<int> v;
    v.push_back(1);
    v.push_back(2);

    ensure(captured([&] { println(empty.size(), empty); }) == "0\n");
    ensure(captured([&] { println(v.size(), v); }) == "2 1 2\n");
    ensure(captured([&] { println(empty, 5); }) == "5\n");
    ensure(captured([&] { println(5, empty, 7); }) == "5 7\n");
    ensure(captured([&] { println(1, 2, 3, 4, 5, 6, empty); }) == "1 2 3 4 5 6\n");
    ensure(captured([&] { println(empty, empty); }) == "\n");
    ensure(captured([&] { println(string(), 5); }) == " 5\n");

    println("ok");
}
