// talon-utilities/ropes/ropes.cpp
/**
 * @file ropes.cpp
 * @brief Extracts printable strings from binaries.
 *
 * In normal mode, outputs only strings not present in ~/.stringignore.json.
 * In training mode (--muffle), updates the ignore list with new strings found,
 * and reports per-source additions.
 *
 * Authors: Matt Heck, R. Talon
 * License: MIT License (see LICENSE file for details)
 *
 * (Now with per-source reporting—because when you expand wildcards, you deserve to know exactly who’s adding noise.)
 */

 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <string>
 #include <unordered_set>
 #include <cstdlib>
 #include <cctype>
 #include <stdexcept>
 
 // (Helper: Extracts contiguous printable strings (min length 4) from a file.)
 std::unordered_set<std::string> extractStringsFromFile(const std::string &filename) {
     std::unordered_set<std::string> result;
     std::ifstream infile(filename, std::ios::binary);
     if (!infile)
         throw std::runtime_error("Unable to open file: " + filename);
 
     std::string current;
     char c;
     while (infile.get(c)) {
         if (std::isprint(static_cast<unsigned char>(c))) {
             current.push_back(c);
         } else {
             if (current.length() >= 4)
                 result.insert(current);
             current.clear();
         }
     }
     if (current.length() >= 4)
         result.insert(current);
     return result;
 }
 
 // (Helper: Writes a set of strings as a JSON array to the specified output path.)
 void writeJSON(const std::unordered_set<std::string> &strings, const std::string &outputPath) {
     std::ofstream outfile(outputPath);
     if (!outfile)
         throw std::runtime_error("Unable to open output file: " + outputPath);
     outfile << "[\n";
     bool first = true;
     for (const auto &s : strings) {
         if (!first)
             outfile << ",\n";
         outfile << "  \"" << s << "\"";
         first = false;
     }
     outfile << "\n]\n";
 }
 
 // (Helper: Loads strings from a JSON file (expects an array of strings).)
 std::unordered_set<std::string> loadIgnoreFile(const std::string &path) {
     std::unordered_set<std::string> ignoreSet;
     std::ifstream infile(path);
     if (!infile)
         return ignoreSet; // No file yet; start with an empty set.
     std::string content((std::istreambuf_iterator<char>(infile)),
                          std::istreambuf_iterator<char>());
     size_t pos = 0;
     while ((pos = content.find('"', pos)) != std::string::npos) {
         size_t start = pos + 1;
         size_t end = content.find('"', start);
         if (end == std::string::npos)
             break;
         std::string s = content.substr(start, end - start);
         ignoreSet.insert(s);
         pos = end + 1;
     }
     return ignoreSet;
 }
 
 int main(int argc, char** argv) {
     const char* home = std::getenv("HOME");
     if (!home) {
         std::cerr << "Error: HOME environment variable not set.\n";
         return 1;
     }
     std::string ignorePath = std::string(home) + "/.stringignore.json";
     auto ignoreSet = loadIgnoreFile(ignorePath);
     size_t initialCount = ignoreSet.size();
 
     if (argc < 2) {
         std::cerr << "Usage: " << argv[0] << " [--muffle] <executable1> [executable2] ...\n";
         return 1;
     }
 
     bool muffle = false;
     int startIdx = 1;
     if (std::string(argv[1]) == "--muffle") {
         muffle = true;
         startIdx = 2;
         if (argc < 3) {
             std::cerr << "Usage: " << argv[0] << " --muffle <executable1> [executable2] ...\n";
             return 1;
         }
     }
 
     if (muffle) {
         // Report current state of the ignore list.
         if (initialCount > 0)
             std::cout << initialCount << " strings in the current ignore list at " << ignorePath << "\n";
         else
             std::cout << "No current ignore list; making one at " << ignorePath << "\n";
 
         // Process each source executable individually.
         for (int i = startIdx; i < argc; ++i) {
             std::unordered_set<std::string> fileStrings;
             try {
                 fileStrings = extractStringsFromFile(argv[i]);
             } catch (const std::exception &e) {
                 std::cerr << "Error processing " << argv[i] << ": " << e.what() << "\n";
                 continue;
             }
             size_t newCount = 0;
             for (const auto &s : fileStrings) {
                 if (ignoreSet.find(s) == ignoreSet.end()) {
                     ignoreSet.insert(s);
                     ++newCount;
                 }
             }
             if (newCount > 0)
                 std::cout << newCount << " new strings added from " << argv[i] << "\n";
             else
                 std::cout << "No new strings added from " << argv[i] << "\n";
         }
         // Write updated ignore list.
         try {
             writeJSON(ignoreSet, ignorePath);
         } catch (const std::exception &e) {
             std::cerr << "Error writing ignore file: " << e.what() << "\n";
             return 1;
         }
         std::cout << ignoreSet.size() << " strings now in the ignore list.\n";
     } else {
         // Normal mode: output strings not in the ignore list.
         for (int i = startIdx; i < argc; ++i) {
             try {
                 auto fileStrings = extractStringsFromFile(argv[i]);
                 for (const auto &s : fileStrings)
                     if (ignoreSet.find(s) == ignoreSet.end())
                         std::cout << s << "\n";
             } catch (const std::exception &e) {
                 std::cerr << "Error processing " << argv[i] << ": " << e.what() << "\n";
             }
         }
     }
 
     return 0;
 }
 
