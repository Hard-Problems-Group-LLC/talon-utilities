// talon-utilities/ropes/noiseref__release_cpp_stl.cpp
/**
 * @file noiseref__release_cpp_stl.cpp
 * @brief Minimal C++ program using STL for release reference.
 *
 * Authors: Matt Heck, R. Talon
 * License: MIT License (see LICENSE file for details)
 *
 * (A minimal demo of STL usage in release mode—optimized for speed, not style.)
 */
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec{"Release", "STL", "Reference"};
    for (const auto &s : vec) {
        std::cout << s << "\n";
    }
    return 0;
}
