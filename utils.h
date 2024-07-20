#ifndef UTILS_H_
#define UTILS_H_

#include "bits/stdc++.h"
#include <filesystem>

enum PrintFormat { Prompt, Solution };

// map for keeping the names of directories
// promptInputDirectory - directory for in files formatted for prompt
// promptInputDirectory - directory for in files formatted for the solution
const std::map<std::string, std::string> dirs = {{"promptInputDirectory", "in"},
                                                 {"solutionInputDirectory", "solution-in"}};

template <typename F, typename S>
std::ostream &operator<<(std::ostream &os, const std::pair<F, S> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "{";
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            os << ", ";
        }
        os << *it;
    }
    return os << "}";
}

void println() { std::cout << '\n'; }
template <typename Head, typename... Tail>
void println(Head H, Tail... T) {
    std::cout << ' ' << H;
    println(T...);
}

void setupDirectories() {
    for (const auto &dir : dirs) {
        if (!std::filesystem::create_directory(dir.second)) {
            // Only warning, because you can fail to create a directory if it exists.
            std::cerr << "Warning: Could not create directory " << dir << std::endl;
        }
    }
}

#endif
