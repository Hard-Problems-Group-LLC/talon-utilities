# Makefile
# Talon Utilities - ropes
# Authors: Matt Heck, R. Talon
# License: MIT License
#
# Build the ropes tool, noise-reference binaries (for filtering), and rabbit programs.
# After building, generate the ignore list by running:
#   ./ropes --muffle $(NOISEREFS)
# (Because someone has to filter out the compiler’s endless chatter—rabbits just hop along.)
#
# The "check" target runs ropes in normal mode on the rabbit binaries and verifies that
# the expected messages ("I'm a C rabbit!" and "I'm a C++ rabbit!") appear in the output.
# This ensures that training mode (which updates the ignore list) is not applied to the rabbits.

CXX = g++
CC = gcc
CXXFLAGS_DEBUG = -std=c++17 -O0 -g
CXXFLAGS_RELEASE = -std=c++17 -O2 -DNDEBUG
CFLAGS_DEBUG = -O0 -g
CFLAGS_RELEASE = -O2 -DNDEBUG

TARGET = ropes
NOISEREFS = noiseref__debug_c noiseref__release_c noiseref__debug_cpp noiseref__release_cpp \
            noiseref__debug_cpp_stl noiseref__release_cpp_stl noiseref__boost_debug noiseref__boost_release
STRINGIGNORE = $(HOME)/.stringignore.json

# Rabbit targets (not used for .stringignore.json)
RABBIT_C_DEBUG = rabbit_debug_c
RABBIT_C_RELEASE = rabbit_release_c
RABBIT_CPP_DEBUG = rabbit_debug_cpp
RABBIT_CPP_RELEASE = rabbit_release_cpp

all: $(TARGET) $(NOISEREFS) generate_ignore $(RABBIT_C_DEBUG) $(RABBIT_C_RELEASE) $(RABBIT_CPP_DEBUG) $(RABBIT_CPP_RELEASE)

$(TARGET): ropes.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) ropes.cpp

noiseref__debug_c: noiseref__debug_c.c
	$(CC) $(CFLAGS_DEBUG) -o noiseref__debug_c noiseref__debug_c.c

noiseref__release_c: noiseref__release_c.c
	$(CC) $(CFLAGS_RELEASE) -o noiseref__release_c noiseref__release_c.c

noiseref__debug_cpp: noiseref__debug_cpp.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -o noiseref__debug_cpp noiseref__debug_cpp.cpp

noiseref__release_cpp: noiseref__release_cpp.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -o noiseref__release_cpp noiseref__release_cpp.cpp

noiseref__debug_cpp_stl: noiseref__debug_cpp_stl.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -o noiseref__debug_cpp_stl noiseref__debug_cpp_stl.cpp

noiseref__release_cpp_stl: noiseref__release_cpp_stl.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -o noiseref__release_cpp_stl noiseref__release_cpp_stl.cpp

noiseref__boost_debug: noiseref__boost_debug.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -o noiseref__boost_debug noiseref__boost_debug.cpp

noiseref__boost_release: noiseref__boost_release.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -o noiseref__boost_release noiseref__boost_release.cpp

# Training mode: update ignore list using only the built noise-reference binaries.
generate_ignore: $(TARGET) $(NOISEREFS)
	./$(TARGET) --muffle $(NOISEREFS)

# Rabbit targets (separate from muffle noise references)
$(RABBIT_C_DEBUG): rabbit.c
	$(CC) $(CFLAGS_DEBUG) -o $(RABBIT_C_DEBUG) rabbit.c

$(RABBIT_C_RELEASE): rabbit.c
	$(CC) $(CFLAGS_RELEASE) -o $(RABBIT_C_RELEASE) rabbit.c

$(RABBIT_CPP_DEBUG): rabbit.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -o $(RABBIT_CPP_DEBUG) rabbit.cpp

$(RABBIT_CPP_RELEASE): rabbit.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -o $(RABBIT_CPP_RELEASE) rabbit.cpp

# Check target: run ropes in normal mode on the rabbit binaries.
# The output should include the rabbit messages, and no new strings should be added to the ignore list.
check: $(RABBIT_C_DEBUG) $(RABBIT_C_RELEASE) $(RABBIT_CPP_DEBUG) $(RABBIT_CPP_RELEASE) $(TARGET)
	@echo "Running tests on rabbit binaries in normal mode..."
	@./$(TARGET) $(RABBIT_C_DEBUG) $(RABBIT_C_RELEASE) $(RABBIT_CPP_DEBUG) $(RABBIT_CPP_RELEASE) > test_output.txt
	@echo "Verifying test output..."
	@if grep -q "I'm a C rabbit!" test_output.txt && grep -q "I'm a C++ rabbit!" test_output.txt; then \
	    echo "All rabbit tests passed."; \
	else \
	    echo "Test failed: expected rabbit strings not found in output."; \
	    rm -f test_output.txt; \
	    exit 1; \
	fi
	@rm -f test_output.txt

clean:
	rm -f $(TARGET) $(NOISEREFS) $(STRINGIGNORE) $(RABBIT_C_DEBUG) $(RABBIT_C_RELEASE) $(RABBIT_CPP_DEBUG) $(RABBIT_CPP_RELEASE)
