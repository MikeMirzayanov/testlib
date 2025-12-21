#include "testlib.h"
#include <vector>
#include <array>
using namespace std;

typedef int C_array[5];
static_assert(!is_range<C_array, C_array>::value, "C array should not be a range");

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);

    // print chars
    println("Hello world");
    println("Hello", "world", "and TestLib");
    println("Hello TestLib", VERSION);

    // print ints
    println(1);
    println(2);
    println(1, 2, 3, 5, 6);

    // mix ints and chars
    const int start = 2010, end = 2025;
    println("Codeforces (c) Copyright", start, '-', end, "Mike Mirzayanov");

    // print arrays, std::array, and std::vector
    int a[5] = {1, 2, 3, 5, 6};
    println(a);
    array<int, 5> arr = {7, 8, 10, 11, 15};
    println(arr);
    vector<int> vec = {10, 20, 30, 50, 60};
    println(vec);

    // print some arrays at the same time
    println(a, arr, vec);
    println(a, a);
    println(arr, arr);
    println(vec, vec);

    // mix with chars
    println("a:", a);
    println("arr:", arr);
    println("vec:", vec);

    // print ranges
    println(a, a + sizeof(a) / sizeof(a[0]));
    println(arr.begin() + 1, arr.end());
    println(vec.begin(), vec.end() - 2);

    // print ranges mixed
    println(a, a + sizeof(a) / sizeof(a[0]), arr.begin(), arr.end());
    
    // print ranges mixed with chars
    println("a is", a, a + sizeof(a) / sizeof(a[0]), "while arr is", arr.begin(), arr.end());

    // print strings
    vector<string> cfKing = {"Yuchen Du", "Lingyu Jiang", "Qiwen Xu"};
    println(cfKing, "form the team CF King");
    println(cfKing[1]);

    // print super-long message
    println("This", "is", "a", "super", "long", "message", "that", "will", "make", "previous", "println", "compile", "error", "but", "now", "it", "will", "be", "safe");
}
