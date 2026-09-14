/**
 * @file ESP32 driver.
 */
#pragma once

#include <cstdint>

namespace driver::gpio
{
class Esp32 final
{
public:
    explicit Esp32(std::uint8_t pin) noexcept;

    void write(bool state) noexcept;

    bool read() const noexcept;

private:
    const std::uint8_t myPin;
};
} // namespace driver::gpio
