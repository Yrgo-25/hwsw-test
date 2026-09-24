# Microcontroller and CPU frequency.
MCU   := atmega328p
F_CPU := 16000000UL

# Source and include directories.
SOURCE_DIR := source
INC_DIR    := include

# Build directory and firmware targets.
BUILD_DIR := bin
ELF       := $(BUILD_DIR)/firmware.elf
HEX       := $(BUILD_DIR)/firmware.hex

# Source files - update this list as new source files are added to the system.
# The virtual hardware platform (source/arch/test) is only used by the test suite.
SOURCE_FILES := $(SOURCE_DIR)/driver/adc/atmega328p.cpp \
                $(SOURCE_DIR)/driver/eeprom/atmega328p.cpp \
                $(SOURCE_DIR)/driver/gpio/atmega328p.cpp \
                $(SOURCE_DIR)/driver/serial/atmega328p.cpp \
                $(SOURCE_DIR)/driver/tempsensor/tmp36.cpp \
                $(SOURCE_DIR)/driver/timer/atmega328p.cpp \
                $(SOURCE_DIR)/driver/watchdog/atmega328p.cpp \
                $(SOURCE_DIR)/logic/logic.cpp \
                $(SOURCE_DIR)/utils/utils.cpp \
                $(SOURCE_DIR)/main.cpp

# AVR toolchain.
CXX_COMPILER := avr-g++
OBJCOPY      := avr-objcopy
SIZE         := avr-size

# AVR compiler flags.
CXX_FLAGS := -std=c++17 -Os -Werror -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I$(INC_DIR) \
             -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti \
             -fno-threadsafe-statics

# AVR linker flags, remove unused sections to reduce the firmware size.
LD_FLAGS := -Wl,--gc-sections

.PHONY: default build test clean

# Build the firmware and run the test suite as default.
default: build test

# Build the AVR firmware (requires avr-gcc, e.g. sudo apt -y install gcc-avr avr-libc).
build: $(HEX)

$(ELF): $(SOURCE_FILES) | $(BUILD_DIR)
	@$(CXX_COMPILER) $(CXX_FLAGS) $(SOURCE_FILES) -o $@ $(LD_FLAGS)
	@$(SIZE) $@

$(HEX): $(ELF)
	@$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BUILD_DIR):
	@mkdir -p $@

# Build and run the test suite in test/.
test:
	@$(MAKE) -C test

# Clean the firmware and the test suite.
clean:
	@rm -rf $(BUILD_DIR)
	@$(MAKE) -C test clean
