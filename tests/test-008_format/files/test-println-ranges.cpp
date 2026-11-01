#include "testlib.h"

#include <deque>
#include <iterator>
#include <list>
#include <set>
#include <sstream>

#if defined(__has_include)
#if ((__cplusplus > 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)) && __has_include(<print>)
#include <print>
#endif
#endif

using namespace std;

template<typename Action>
void checkOutput(const std::string &name, const std::string &expected, Action action) {
    std::ostringstream output;
    std::streambuf *previous = std::cout.rdbuf(output.rdbuf());
    action();
    std::cout.rdbuf(previous);
    ensuref(output.str() == expected, "%s: expected '%s', found '%s'",
            name.c_str(), expected.c_str(), output.str().c_str());
}

template<typename Container>
void checkContainerRanges(const std::string &name, Container &values) {
    const Container &constantValues = values;
    const typename Container::reverse_iterator reverseEnd = values.rend();
    checkOutput(name + " forward", "1 2 3\n", [&] { println(values.begin(), values.end()); });
    checkOutput(name + " const", "1 2 3\n", [&] { println(constantValues.begin(), constantValues.end()); });
    checkOutput(name + " mixed const", "1 2 3\n", [&] { println(values.begin(), constantValues.end()); });
    checkOutput(name + " reverse pair", "3 2 1\n", [&] { println(values.rbegin(), values.rend()); });
    checkOutput(name + " explicit reverse", "3 2 1\n", [&] { println(values.end(), values.rend()); });
    checkOutput(name + " const reverse end", "3 2 1\n", [&] { println(values.end(), reverseEnd); });
    checkOutput(name + " empty", "\n", [&] { println(values.begin(), values.begin()); });
    checkOutput(name + " empty reverse", "\n", [&] { println(values.end(), values.rbegin()); });
    checkOutput(name + " single element", "1\n", [&] { println(values.begin(), std::next(values.begin())); });
    checkOutput(name + " subrange", "2 3\n", [&] { println(std::next(values.begin()), values.end()); });
}

// Not for std::set: libstdc++ 4.9 cannot dereference move_iterator over constant iterators (LWG 2106).
template<typename Container>
void checkMoveRanges(const std::string &name, Container &values) {
    const std::move_iterator<typename Container::iterator> moveEnd = std::make_move_iterator(values.end());
    checkOutput(name + " move pair", "1 2 3\n", [&] {
        println(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
    });
    checkOutput(name + " explicit move", "1 2 3\n", [&] {
        println(values.begin(), std::make_move_iterator(values.end()));
    });
    checkOutput(name + " const move end", "1 2 3\n", [&] { println(values.begin(), moveEnd); });
    checkOutput(name + " empty move", "\n", [&] {
        println(values.begin(), std::make_move_iterator(values.begin()));
    });
}

// The constructor deliberately accepts only an lvalue: __println_range calls B(begin).
struct LvalueConstructedIterator {
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef std::ptrdiff_t difference_type;
    typedef int *pointer;
    typedef int &reference;

    std::vector<int>::iterator value;

    explicit LvalueConstructedIterator(std::vector<int>::iterator &value) : value(value) {}

    int &operator*() const { return *value; }
    int *operator->() const { return &*value; }
    LvalueConstructedIterator &operator++() { ++value; return *this; }
    LvalueConstructedIterator operator++(int) {
        LvalueConstructedIterator previous = *this;
        ++*this;
        return previous;
    }
    bool operator==(const LvalueConstructedIterator &other) const { return value == other.value; }
    bool operator!=(const LvalueConstructedIterator &other) const { return value != other.value; }
};

struct QualifiedRangeBegin {
    std::vector<int>::iterator value;

    operator std::vector<int>::iterator() & { return value; }
    operator std::vector<int>::iterator() const && { return value; }
};

struct MutablePointerIterator {
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef std::ptrdiff_t difference_type;
    typedef int *pointer;
    typedef int &reference;

    int *value;

    explicit MutablePointerIterator(int *value = NULL) : value(value) {}

    int &operator*() const { return *value; }
    int *operator->() const { return value; }
    MutablePointerIterator &operator++() { ++value; return *this; }
    MutablePointerIterator operator++(int) {
        MutablePointerIterator previous = *this;
        ++*this;
        return previous;
    }
    bool operator==(const MutablePointerIterator &other) const { return value == other.value; }
    bool operator!=(const MutablePointerIterator &other) const { return value != other.value; }
};

std::ostream &operator<<(std::ostream &out, const MutablePointerIterator &) {
    return out << "iterator";
}

template<typename T>
struct NestedType {
    typedef typename T::type type;
};

struct ExplicitValue {
    ExplicitValue() {}

    // This constructor is only intended for types with a nested type, not for the first println argument.
    template<typename T>
    explicit ExplicitValue(T, typename NestedType<T>::type * = NULL) {}
};

std::ostream &operator<<(std::ostream &out, const ExplicitValue &) {
    return out << "value";
}

struct NonCopyableRangeBegin {
    int *value;

    explicit NonCopyableRangeBegin(int *value) : value(value) {}
    NonCopyableRangeBegin(const NonCopyableRangeBegin &) = delete;
    explicit operator int *() & { return value; }
};

std::ostream &operator<<(std::ostream &out, const NonCopyableRangeBegin &) {
    return out << "begin";
}

struct MoveOnlyRangeEnd : MutablePointerIterator {
    explicit MoveOnlyRangeEnd(int *value) : MutablePointerIterator(value) {}
    explicit MoveOnlyRangeEnd(const std::string &) : MutablePointerIterator() {}
    MoveOnlyRangeEnd(const MoveOnlyRangeEnd &) = delete;
    MoveOnlyRangeEnd(MoveOnlyRangeEnd &&) = default;
};

struct ExplicitTextBegin {
    const char *value;

    explicit operator const char *() const { return value; }
};

std::ostream &operator<<(std::ostream &out, const ExplicitTextBegin &) {
    return out << "begin";
}

struct IncompleteValue;
std::ostream &operator<<(std::ostream &out, const IncompleteValue &value);
IncompleteValue &incompleteValue();

// IncompleteValue is still incomplete here: println must not inspect the first argument
// when the second one is not an iterator.
void printIncompleteValue() {
    ::println(incompleteValue(), 5);
}

struct IncompleteValue {
    int value;
};

std::ostream &operator<<(std::ostream &out, const IncompleteValue &value) {
    return out << "incomplete" << value.value;
}

IncompleteValue &incompleteValue() {
    static IncompleteValue value = {7};
    return value;
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    std::vector<int> values = {1, 2, 3};
    std::list<int> listValues(values.begin(), values.end());
    std::deque<int> dequeValues(values.begin(), values.end());
    std::set<int> setValues(values.begin(), values.end());
    checkContainerRanges("vector", values);
    checkContainerRanges("list", listValues);
    checkContainerRanges("deque", dequeValues);
    checkContainerRanges("set", setValues);
    checkMoveRanges("vector", values);
    checkMoveRanges("list", listValues);
    checkMoveRanges("deque", dequeValues);

    int numbers[] = {4, 5, 6};
    const int constantNumbers[] = {7, 8, 9};
    const int *constantBegin = numbers;
    checkOutput("array", "4 5 6\n", [&] { println(numbers, numbers + 3); });
    checkOutput("const array", "7 8 9\n", [&] { println(constantNumbers, constantNumbers + 3); });
    checkOutput("pointers", "4 5 6\n", [&] { println(numbers + 0, numbers + 3); });
    checkOutput("const begin pointer", "4 5 6\n", [&] { println(constantBegin, numbers + 3); });
    checkOutput("const end pointer", "4 5 6\n", [&] { println(numbers + 0, constantBegin + 3); });
    checkOutput("pointer subrange", "5 6\n", [&] { println(numbers + 1, numbers + 3); });
    checkOutput("empty pointer range", "\n", [&] { println(numbers + 3, numbers + 3); });
    checkOutput("pointer to reverse adapter", "6 5 4\n", [&] {
        println(numbers + 3, std::reverse_iterator<int *>(numbers));
    });

    typedef std::vector<int>::iterator Iterator;
    typedef std::vector<int>::reverse_iterator ReverseIterator;
    checkOutput("explicit template arguments", "3 2 1\n", [&] {
        ::println<Iterator, ReverseIterator>(values.end(), values.rend());
    });
    void (*printReverse)(const Iterator &, const ReverseIterator &) = &::println<Iterator, ReverseIterator>;
    checkOutput("function pointer", "3 2 1\n", [&] { printReverse(values.end(), values.rend()); });
    Iterator end = values.end();
    LvalueConstructedIterator customEnd(end);
    const LvalueConstructedIterator constantCustomEnd(end);
    checkOutput("explicit lvalue constructor", "1 2 3\n", [&] { println(values.begin(), customEnd); });
    checkOutput("const explicit lvalue constructor", "1 2 3\n", [&] { println(values.begin(), constantCustomEnd); });

    const char *text = "text";
    const char *const constantText = text;
    char mutableText[] = "mutable";
    char *mutablePointer = mutableText;
    std::string word = "word";
    checkOutput("zero and text", "0 text\n", [&] { println(0, text); });
    checkOutput("negative and text", "-1 text\n", [&] { println(-1, text); });
    checkOutput("bool and text", "1 text\n", [&] { println(true, text); });
    checkOutput("string and text", "word text\n", [&] { println(word, text); });
    checkOutput("string and const pointer", "word text\n", [&] { println(word, constantText); });
    checkOutput("integer and mutable pointer", "1 mutable\n", [&] { println(1, mutablePointer); });
    checkOutput("string and mutable pointer", "word mutable\n", [&] { println(word, mutablePointer); });
    checkOutput("container and text", "1 2 3 text\n", [&] { println(values, text); });
    checkOutput("two C strings", "text text\n", [&] { println(text, text); });
    checkOutput("two literals", "left right\n", [&] { println("left", "right"); });
    checkOutput("explicit scalar arguments", "1 text\n", [&] { ::println<int, const char *>(1, text); });

    const QualifiedRangeBegin qualifiedBegin = {values.begin()};
    checkOutput("explicit const begin type", "1 2 3\n", [&] {
        ::println<const QualifiedRangeBegin, Iterator>(qualifiedBegin, values.end());
    });

    // The array binds to const A&, so __println_range would receive const int*, not int*.
    // Keep printing two values when the end iterator requires a mutable pointer.
    MutablePointerIterator mutableEnd(numbers + 3);
    std::ostringstream arrayAndIterator;
    arrayAndIterator << static_cast<const void *>(numbers) << " iterator\n";
    checkOutput("array and mutable iterator", arrayAndIterator.str(), [&] { println(numbers, mutableEnd); });
    checkOutput("pointer and mutable iterator", "4 5 6\n", [&] { println(numbers + 0, mutableEnd); });

    ExplicitValue explicitValue;
    checkOutput("integer and non-iterator", "1 value\n", [&] { println(1, explicitValue); });
    checkOutput("literal and non-iterator", "literal value\n", [&] { println("literal", explicitValue); });

    // Constructing an iterator is not enough: __println_range must also copy both arguments.
    NonCopyableRangeBegin nonCopyableBegin(numbers);
    std::ostringstream beginAndPointer;
    beginAndPointer << "begin " << static_cast<const void *>(numbers + 3) << "\n";
    checkOutput("noncopyable begin", beginAndPointer.str(), [&] { println(nonCopyableBegin, numbers + 3); });
    MoveOnlyRangeEnd moveOnlyEnd(numbers + 3);
    checkOutput("move-only end", "word iterator\n", [&] { println(word, moveOnlyEnd); });

    // B(begin) may use an explicit conversion operator of the first argument.
    const ExplicitTextBegin textBegin = {"abc"};
    checkOutput("explicit conversion operator", "a b c\n", [&] { println(textBegin, textBegin.value + 3); });
    checkOutput("incomplete first argument", "incomplete7 5\n", printIncompleteValue);

    println("println range scenarios passed");
}
