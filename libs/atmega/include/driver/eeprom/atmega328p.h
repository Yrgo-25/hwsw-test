/**
 * @brief EEPROM stream for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/eeprom/interface.h"

namespace driver
{
namespace eeprom
{
/**
 * @brief EEPROM stream for ATmega328P.
 *
 *        Use the singleton design pattern to ensure only one EEPROM instance exists,
 *        reflecting the hardware limitation of a single EEPROM on the MCU.
 */
class Atmega328p final : public Interface
{
public:
    /**
     * @brief Get the singleton EEPROM instance.
     *
     * @return Reference to the singleton EEPROM instance.
     */
    static Interface& getInstance() noexcept;

    /**
     * @brief Get the size of the EEPROM.
     *
     * @return The size of the EEPROM in bytes.
     */
    uint16_t size() const noexcept override;

    /**
     * @brief Check whether the EEPROM stream is initialized.
     *
     * @return True if the EEPROM stream is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicate whether the EEPROM stream is enabled.
     *
     * @return True if the EEPROM stream is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of EEPROM stream.
     *
     * @param[in] enable Indicate whether to enable the EEPROM stream.
     */
    void setEnabled(bool enable) noexcept override;

    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    Atmega328p() noexcept;
    ~Atmega328p() noexcept override = default;
    bool isAddressValid(uint16_t address, uint8_t dataSize) const noexcept override;
    void writeByte(uint16_t address, uint8_t data) noexcept override;
    uint8_t readByte(uint16_t address) const noexcept override;

    /** Indicate whether the EEPROM stream is enabled. */
    bool myEnabled;
};
} // namespace eeprom
} // namespace driver
