/**
 * @brief EEPROM stream implementation details for ATmega328P.
 */
#include "driver/eeprom/atmega328p.h"
#include "arch/avr/hw_platform.h"
#include "utils/utils.h"

namespace driver
{
namespace eeprom
{
namespace
{
/**
 * @brief Structure of ATmega328P EEPROM parameters.
 */
struct EepromParam
{
    /** Size of the EEPROM in bytes. */
    static constexpr uint16_t Size{1024U};

    /** Highest EEPROM address. */
    static constexpr uint16_t MaxAddress{Size - 1U};
};
} // namespace

// -----------------------------------------------------------------------------
Interface& Atmega328p::getInstance() noexcept
{
    // Create and initialize the singleton EEPROM instance (once only).
    static Atmega328p myInstance{};

    // Return a reference to the singleton EEPROM instance, cast to the corresponding interface.
    return myInstance;
}

// -----------------------------------------------------------------------------
uint16_t Atmega328p::size() const noexcept { return EepromParam::Size; }

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Atmega328p::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Atmega328p::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p() noexcept
    : myEnabled{false}
{}

// -----------------------------------------------------------------------------
bool Atmega328p::isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept
{
    return EepromParam::MaxAddress >= address + dataSize;
}

// -----------------------------------------------------------------------------
void Atmega328p::writeByte(const uint16_t address, const uint8_t data) noexcept
{
    // Wait until EEPROM is ready to send the next byte.
    while (utils::read(EECR, EEPE))
        ;

    // Set the address and data to write.
    EEAR = address;
    EEDR = data;

    // Perform write, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    utils::set(EECR, EEMPE);
    utils::set(EECR, EEPE);

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
uint8_t Atmega328p::readByte(const uint16_t address) const noexcept
{
    // Wait until EEPROM is ready to read the next byte.
    while (utils::read(EECR, EEPE))
        ;

    // Set the address from which to read.
    EEAR = address;

    // Read and return the value of the given address.
    utils::set(EECR, EERE);
    return EEDR;
}
} // namespace eeprom
} // namespace driver
