/**
 * @brief Unit tests for the Atmega328p EEPROM.
 */
#include <cstdint>
#include <limits>

#include "yrgo/test/test.h"

#include "arch/avr/hw_platform.h"
#include "driver/eeprom/atmega328p.h"
#include "utils/utils.h"

#ifdef TESTSUITE

namespace driver
{
namespace
{
/** EEPROM size in bytes. */
constexpr std::uint16_t EepromSize{1024U};

// -----------------------------------------------------------------------------
constexpr bool isAddrValid(const std::uint16_t addr) noexcept { return EepromSize > addr; }

// -----------------------------------------------------------------------------
void writeToEeprom(eeprom::Interface& eeprom, const std::uint16_t addr)
{
    constexpr std::uint8_t zero{0U};

    // Case 1 - Try EEPROM write when enabled.
    {
        // Enable EEPROM and clear the associated registers.
        eeprom.setEnabled(true);
        EEAR = zero;
        EEDR = zero;
        EECR = zero;

        // Try to write to EEPROM, store the operation result.
        constexpr std::uint8_t expectedData{100U};
        const bool success{eeprom.write(addr, expectedData)};

        // If the address is valid, expect the EEPROM write to be successful.
        if (isAddrValid(addr))
        {
            // Expect the given address and data to be written to the corresponding registers.
            constexpr std::uint8_t expectedEecrOnWrite{(1U << EEMPE) | (1U << EEPE)};
            EXPECT_TRUE(success);
            EXPECT_EQ(EEAR, addr);
            EXPECT_EQ(EEDR, expectedData);
            EXPECT_EQ(EECR, expectedEecrOnWrite);
        }
        else
        {
            EXPECT_FALSE(success);
            EXPECT_EQ(EEAR, zero);
            EXPECT_EQ(EEDR, zero);
            EXPECT_EQ(EECR, zero);
        }
    }

    // Case 2 - Try EEPROM write when disabled.
    {
        // Disable EEPROM and clear the associated registers.
        eeprom.setEnabled(false);
        EEAR = zero;
        EEDR = zero;
        EECR = zero;

        // Try to write another value, expect the operation to fail and the registers to be zero.
        constexpr std::uint8_t data2{200U};
        const bool success{eeprom.write(addr, data2)};
        EXPECT_FALSE(success);
        EXPECT_EQ(EEAR, zero);
        EXPECT_EQ(EEDR, zero);
        EXPECT_EQ(EECR, zero);
    }
}

// -----------------------------------------------------------------------------
void readFromEeprom(eeprom::Interface& eeprom, const std::uint16_t addr)
{
    constexpr std::uint8_t zero{0U};

    // Case 1 - Try EEPROM read when enabled.
    {
        // Enable EEPROM and clear the associated registers.
        eeprom.setEnabled(true);
        EEAR = zero;
        EECR = zero;

        // Set a non-zero data value to read from EEPROM.
        constexpr std::uint8_t expectedData{50U};
        EEDR = expectedData;

        // Try to read from EEPROM, store the operation result.
        std::uint8_t actualData{};
        const bool success{eeprom.read(addr, actualData)};

        // If the address is valid, expect the EEPROM read to be successful.
        if (isAddrValid(addr))
        {
            constexpr std::uint8_t expectedEecrOnRead{(1U << EERE)};
            EXPECT_TRUE(success);
            EXPECT_EQ(EEAR, addr);
            EXPECT_EQ(EECR, expectedEecrOnRead);
            EXPECT_EQ(actualData, expectedData);
        }
        else
        {
            EXPECT_FALSE(success);
            EXPECT_EQ(EEAR, zero);
            EXPECT_EQ(EECR, zero);
            EXPECT_EQ(actualData, zero);
        }
    }

    // Case 2 - Try EEPROM read when disabled.
    {
        // Disable EEPROM and clear the associated registers.
        eeprom.setEnabled(false);
        EEAR = zero;
        EECR = zero;

        // Set a non-zero data value to read from EEPROM.
        constexpr std::uint8_t expectedData{175U};
        EEDR = expectedData;

        // Try to read again, expect the operation to fail.
        std::uint8_t actualData{};
        const bool success{eeprom.read(addr, actualData)};

        EXPECT_FALSE(success);
        EXPECT_EQ(EEAR, zero);
        EXPECT_EQ(EECR, zero);
        EXPECT_EQ(actualData, zero);
    }
}

/**
 * @brief EEPROM write test.
 *
 *        Verify that the EEPROM performs write operations correctly when enabled.
 */
TEST(Eeprom_Atmega328p, Write)
{
    // Expect the EEPROM to be initialized correctly.
    eeprom::Interface& eeprom{eeprom::Atmega328p::getInstance()};
    EXPECT_TRUE(eeprom.isInitialized());
    EXPECT_EQ(eeprom.size(), EepromSize);

    constexpr std::size_t stepVal{10U};

    // Systematically test EEPROM write operations across a range of addresses.
    for (std::size_t i{}; i < std::numeric_limits<std::uint16_t>::max(); i += stepVal)
    {
        const std::uint16_t addr{static_cast<std::uint16_t>(i)};
        writeToEeprom(eeprom, addr);
    }
}

/**
 * @brief EEPROM read test.
 *
 *        Verify that the EEPROM performs read operations correctly when enabled.
 */
TEST(Eeprom_Atmega328p, Read)
{
    // Expect the EEPROM to be initialized correctly.
    eeprom::Interface& eeprom{eeprom::Atmega328p::getInstance()};
    EXPECT_TRUE(eeprom.isInitialized());
    EXPECT_EQ(eeprom.size(), EepromSize);

    constexpr std::size_t stepVal{10U};

    // Systematically test EEPROM read operations across a range of addresses.
    for (std::size_t i{}; i < std::numeric_limits<std::uint16_t>::max(); i += stepVal)
    {
        const std::uint16_t addr{static_cast<std::uint16_t>(i)};
        readFromEeprom(eeprom, addr);
    }
}
} // namespace
} // namespace driver

#endif /** TESTSUITE */
