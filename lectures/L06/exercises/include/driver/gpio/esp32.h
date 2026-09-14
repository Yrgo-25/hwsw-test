/**
 * @file ESP32 driver.
 */
#pragma once

#include <cstdint>

namespace driver::gpio
{
/**
 * @brief ESP32 driver.
 *
 * @note This is not a real driver, but a minimal example for demonstrating how ESP-IDF's
 *       C functions can be mocked. It only compiles against the mock, see the constructor.
 */
class Esp32 final
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] pin GPIO pin number.
     */
    explicit Esp32(std::uint8_t pin) noexcept;

    /**
     * @brief Set GPIO state.
     *
     * @param[in] state GPIO state (true = high, false = low).
     */
    void write(bool state) noexcept;

    /**
     * @brief Read GPIO state.
     *
     * @return True if high, false if low.
     */
    bool read() const noexcept;

private:
    /** GPIO pin number. */
    const std::uint8_t myPin;
};
} // namespace driver::gpio
