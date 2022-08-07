#ifndef _TEST_H_
#define _TEST_H_

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cstdlib>
#include <iostream>

struct test {
    std::string name;
    std::function<void()> f;
};

std::vector<test> tests;

struct test_holder {
    test_holder(const test& t) {
        tests.push_back(t);
    }
};

#define TEST(f) \
    void test_##f(); \
    test test_item##f = {#f, test_##f}; \
    test_holder test_holder##f(test_item##f); \
    void test_##f() \

#ifdef _MSC_VER
#   define TEST_NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define TEST_NORETURN __attribute__ ((noreturn))
#else
#   define TEST_NORETURN
#endif

TEST_NORETURN void error(std::string message) {
    std::cout << "ERROR: " << message << std::endl;
    exit(1);
}

void ensure_exit(int exit_code, std::function<void()> f) {
    try {
        f();
    } catch (exit_exception& e) {
        if (e.getExitCode() != exit_code) {
            error("Expected exit code " + std::to_string(exit_code) + " but found " + std::to_string(e.getExitCode()));
        } else
            return;
    } catch (...) {
        error("Expected exit_exception, but got other exception");
    }
    error("Expected exit_exception, but not exception got");
}

void run_tests() {
    for (const auto& t: tests) {
        std::string title = "Running test '" + t.name + "'";
        while (title.length() < 100)
            title += ' ';
        std::cout << title;

        try {
            t.f();
        } catch (exit_exception& e) {
            error("Teslib exited with code " + std::to_string(e.getExitCode()));
        } catch (...) {
            error("Unexpected exception");
            std::exit(1);
        }
        
        std::cout << "OK" << std::endl;
    }

    std::cout << std::endl << "SUCCESS " << tests.size() << " tests passed." << std::endl;
}

#endif
