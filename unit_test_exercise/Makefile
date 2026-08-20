# Testsuite target.
TARGET := testsuite

# Test files - update this list as new test files are added to the exercise.
TEST_FILES := test/driver/gpio/stub_test.cpp \
              test/driver/timer/stub_test.cpp \
              test/testsuite.cpp \

# Directory containing the yrgo::test framework.
YRGO_TEST_DIR := ../libs/test
YRGO_TEST_LIB := $(YRGO_TEST_DIR)/libyrgo_test.a

# C++ compiler.
CXX_COMPILER = g++

# C++ compiler flags.
CXX_FLAGS = -std=c++17 -Werror -Wall -I. -I$(YRGO_TEST_DIR)/include

.PHONY: default build run clean

# Build and run the test suite as default:
default: build run

# Build the yrgo::test library.
$(YRGO_TEST_LIB):
	@$(MAKE) -C $(YRGO_TEST_DIR) lib

# Build the test suite.
build: $(YRGO_TEST_LIB)
	@$(CXX_COMPILER) $(TEST_FILES) -o $(TARGET) $(CXX_FLAGS) -L$(YRGO_TEST_DIR) -lyrgo_test

# Run the test suite.
run: build
	@./$(TARGET)

# Clean the test suite and the yrgo::test library.
clean:
	@rm -f $(TARGET)
	@$(MAKE) -C $(YRGO_TEST_DIR) clean
