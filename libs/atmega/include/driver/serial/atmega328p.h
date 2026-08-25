/**
 * @brief Serial driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/serial/interface.h"

namespace driver
{
namespace serial
{
/**
 * @brief Serial driver for ATmega328P.
 *
 *        Use the singleton design pattern to ensure only one serial device instance exists,
 *        reflecting the hardware limitation of a single serial port on the MCU.
 *
 *        The configured baud rate is set to 9600 bps.
 */
class Atmega328p final : public Interface
{
public:
    /**
     * @brief Get the singleton serial instance.
     *
     * @return Reference to the singleton serial instance.
     */
    static Interface& getInstance() noexcept;

    /**
     * @brief Get the baud rate of the serial device.
     *
     * @return The baud rate in bps (bits per second).
     */
    uint32_t baudRate_bps() const noexcept override;

    /**
     * @brief Check whether the serial device is initialized.
     *
     * @return True if the device is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the serial device is enabled.
     *
     * @return True if the serial device is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of serial device.
     *
     * @param[in] enable Indicate whether to enable the device.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Read data from the serial port.
     *
     * @param[out] buffer Read buffer.
     * @param[in] size Buffer size in bytes.
     * @param[in] timeout_ms Read timeout. Pass 0 to wait indefinitely.
     *
     * @return The number of read characters, or -1 on error.
     */
    int16_t read(uint8_t* buffer, uint16_t size, uint16_t timeout_ms) const noexcept override;

    Atmega328p(const Atmega328p&)                      = delete; // No copy constructor.
    Atmega328p(Atmega328p&& other) noexcept            = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&)           = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&& other) noexcept = delete; // No move assignment.

private:
    /**
     * @brief Create new serial device.
     */
    Atmega328p() noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept override = default;

    /**
     * @brief Print the given string in the serial terminal.
     *
     * @param[in] str The string to print.
     */
    void print(const char* str) const noexcept override;

    /** Indicate whether serial transmission is enabled. */
    bool myEnabled;
};
} // namespace serial
} // namespace driver
