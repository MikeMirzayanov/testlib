#include "testlib.h"

#if defined(__has_include)
#if ((__cplusplus > 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)) && __has_include(<print>)
#include <print>
#endif
#endif

using namespace std;

std::ostream &callback(std::ostream &out) {
    return out << "invoked";
}

struct AbstractValue {
    virtual ~AbstractValue() {}
    virtual int get() const = 0;
};

struct ConcreteValue : AbstractValue {
    int get() const {
        return 11;
    }
};

std::ostream &operator<<(std::ostream &out, const AbstractValue &value) {
    return out << "abstract-" << value.get();
}

struct NonCopyableValue {
    explicit NonCopyableValue(int value) : value(value) {}

    NonCopyableValue(const NonCopyableValue &) = delete;
    NonCopyableValue &operator=(const NonCopyableValue &) = delete;

    int value;
};

std::ostream &operator<<(std::ostream &out, const NonCopyableValue &value) {
    return out << value.value;
}

struct CopyTrackedValue {
    explicit CopyTrackedValue(int value) : value(value) {}

    CopyTrackedValue(const CopyTrackedValue &other) : value(other.value) {
        ++copies;
    }

    int value;
    static int copies;
};

int CopyTrackedValue::copies = 0;

std::ostream &operator<<(std::ostream &out, const CopyTrackedValue &value) {
    return out << value.value;
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    println("value", std::hex, 255);
    std::cout << std::dec;
    println("callback", callback, 7);

    ConcreteValue concreteValue;
    AbstractValue &abstractValue = concreteValue;
    println("abstract", abstractValue, 7);

    std::vector<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    println(values.begin(), values.end());
    println("a", "b");

    NonCopyableValue nonCopyableValue(9);
    println("noncopyable", nonCopyableValue, 7);

    CopyTrackedValue copyTrackedValue(10);
    println("copies", copyTrackedValue, 7);
    ensure(CopyTrackedValue::copies == 0);

    std::string adlText = "adl";
    println(adlText, 8);

#ifdef __cpp_lib_print
    if (false)
        std::println("{}", 47);
#endif
}
