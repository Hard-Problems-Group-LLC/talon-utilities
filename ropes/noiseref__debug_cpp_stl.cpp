// talon-utilities/ropes/noiseref__debug_cpp_stl.cpp
/**
 * @file noiseref__debug_cpp_stl.cpp
 * @brief Minimal C++ program using STL for debug reference.
 *
 * Authors: Matt Heck, R. Talon
 * License: MIT License (see LICENSE file for details)
 *
 * (A minimal demo of STL usage in debug mode—vector is more exciting than a plain array.)
 */
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec{"Debug", "STL", "Reference"};
    for (const auto &s : vec) {
        std::cout << s << "\n";
    }
    return 0;
}
