// Compile-only probe: instantiates the templates of testlib.h. GCC and Clang check template
// bodies for shadowing where they are defined, MSVC only where they are instantiated.
// fixture-user-globals.cpp includes this file after its globals: keep the names of the locals
// here out of its list.
#include "testlib.h"
#include <list>
#include <set>

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 1000)
        return 0;

    std::vector<int> vec(5, 1);
    std::list<long long> lst(3, 2LL);
    std::set<std::string> strs;
    strs.insert("a");

    std::string str = rnd.next("[a-z]{1,%d}", 5) + rnd.next(std::string("[a-z]"));
    str += pattern("[a-c]{2}").next(rnd);
    (void)rnd.next(10); (void)rnd.next(10u); (void)rnd.next(10LL); (void)rnd.next(10ULL);
    (void)rnd.next(10L); (void)rnd.next(10UL);
    (void)rnd.next(1, 10); (void)rnd.next(1u, 10u); (void)rnd.next(1LL, 10LL); (void)rnd.next(1ULL, 10ULL);
    (void)rnd.next(1L, 10L); (void)rnd.next(1UL, 10UL);
    (void)rnd.next(); (void)rnd.next(2.0); (void)rnd.next(1.0, 2.0);
    (void)rnd.wnext(10, 2); (void)rnd.wnext(10LL, 2); (void)rnd.wnext(2.0, 2); (void)rnd.wnext(2);
    (void)rnd.wnext(10u, 2); (void)rnd.wnext(10ULL, 2); (void)rnd.wnext(10L, 2); (void)rnd.wnext(10UL, 2);
    (void)rnd.wnext(1, 10, 2); (void)rnd.wnext(1u, 10u, 2); (void)rnd.wnext(1LL, 10LL, 2);
    (void)rnd.wnext(1ULL, 10ULL, 2); (void)rnd.wnext(1L, 10L, 2); (void)rnd.wnext(1UL, 10UL, 2);
    (void)rnd.wnext(1.0, 2.0, 2);
    (void)rnd.any(vec); (void)rnd.any(lst); (void)rnd.any(strs); (void)rnd.any(vec.begin(), vec.end());
    (void)rnd.any(str);
    (void)rnd.wany(vec, 1); (void)rnd.wany(lst, -1); (void)rnd.wany(vec.begin(), vec.end(), 2);
    (void)rnd.perm(5); (void)rnd.perm(5LL); (void)rnd.perm(5, 1); (void)rnd.perm(5u, 1LL);
    (void)rnd.distinct(3, 10); (void)rnd.distinct(3, 1LL, 10LL); (void)rnd.distinct(3, 10LL);
    (void)rnd.partition(3, 10); (void)rnd.partition(3, 10LL, 2LL); (void)rnd.partition(3, 10LL);
    shuffle(vec.begin(), vec.end());

    (void)join(vec); (void)join(vec, ','); (void)join(vec.begin(), vec.end()); (void)join(lst.begin(), lst.end(), "--");
    (void)join(strs);
    println(1); println(1, 2); println(1, 2.5, "x"); println(1, 2, 3, 4); println(1, 2, 3, 4, 5);
    println(1, 2, 3, 4, 5, 6); println(1, 2, 3, 4, 5, 6, 7);
    println(vec); println(vec.begin(), vec.end()); println(str, std::string("t")); println(&vec[0], &vec[0] + 1);
    println("ab", "cd"); println('c'); println(strs); println(1, vec);
    const int *cvec = &vec[0];
    println(cvec, cvec + 1);

    (void)vtos(1); (void)vtos(1.5); (void)vtos(std::string("x")); (void)vtos(10ULL);
    (void)toString(1); (void)toString(-7LL);
    (void)toHumanReadableString(1000000); (void)toHumanReadableString(-123456789LL);
    (void)toHumanReadableString(1000000ULL);
    (void)format("%d", 1); (void)format(std::string("%d"), 1); (void)testlib_format_("%d", 1);

    (void)opt<int>(1); (void)opt<long long>("n"); (void)opt<unsigned>("u"); (void)opt<bool>("b");
    (void)opt<double>("d"); (void)opt<std::string>("s");
    (void)opt<double>(2); (void)opt<unsigned>(2);
    (void)opt<int>(1, 5); (void)opt<long long>("n", 5LL); (void)opt<bool>("b", false);
    (void)opt<double>("d", 1.0); (void)opt<std::string>("s", std::string("z"));
    (void)opt<unsigned long long>("ull"); (void)opt<short>("sh"); (void)opt<float>("f"); (void)opt<long double>("ld");
    (void)has_opt("n");
    ensure(true);
    ensuref(true, "%d", 1);
    (void)isEof(1); (void)isEoln('a'); (void)isBlanks(' ');

    std::vector<int> ints = inf.readInts(3, 1, 10, "a");
    (void)ints;
    (void)inf.readLongs(2, 1LL, 2LL); (void)inf.readWords(2); (void)inf.readTokens(2, "[a-z]+");
    (void)inf.readStrictDoubles(2, 0.0, 1.0, 1, 2); (void)inf.readLines(2); (void)inf.readReals(2);
    (void)inf.readUnsignedLongs(2);
    (void)inf.readInt(1, 10, "n"); (void)inf.readLong(1LL, 10LL, "m"); (void)inf.readUnsignedLong(1ULL, 2ULL);
    (void)inf.readDouble(0.0, 1.0); (void)inf.readStrictDouble(0.0, 1.0, 1, 2);
    (void)inf.readWord("[a-z]+", "w"); (void)inf.readLine(pattern("[a-z]*"), "l"); (void)inf.readString();
    (void)inf.readChar('x');
    inf.readSpace();
    inf.readEoln();
    inf.readEof();

    if (argc == 1001) {
        expectedButFound(_wa, 1, 2);
        expectedButFound(_wa, std::string("a"), std::string("b"));
        expectedButFound(_wa, 1.0, 2.0, "x %d", 1);
        expectedButFound(_wa, "a", "b");
        expectedButFound(_wa, 1LL, 2LL);
    }
    if (argc == 1002)
        quitp(1.5, "%d", 1);
    if (argc == 1003)
        quitp(1, "x");
    if (argc == 1004)
        quitf(_ok, "%s", "x");
    quitif(false, _wa, "x");
    return 0;
}
