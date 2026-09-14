# Test target.
TARGET := testsuite

# C++ compiler.
CXX_COMPILER := g++

# Firmware header directory.
FW_INC := ../include

# Firmware source directory.
FW_SRC := ../source

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17 -Iinclude -I$(FW_INC) -DTESTSUITE

# Test files.
TST_FILES := testsuite.cpp \
             source/test/esp32/gpio_mock.c \

# Source files.
SRC_FILES := $(FW_SRC)/driver/gpio/esp32.cpp \

# All source files.
ALL_FILES := $(TST_FILES) $(SRC_FILES)

# Build and run the tests by default.
default: build run

# Build the test suite.
build:
	@$(CXX_COMPILER) $(ALL_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the test suite.
run:
	@./$(TARGET)

# Clean the test suite.
clean:
	@rm -f $(TARGET)
