/**
 * @file ESP32 driver implementation details.
 */
#include <cstdint>
#include <cstdio>

#include "arch/env/hw_platform.h"
#include "driver/gpio/esp32.h"

namespace driver::gpio
{
Esp32::Esp32(const std::uint8_t pin) noexcept
    : myPin{pin}
{
    std::printf("Hallå från YRGO!\n");
}

void Esp32::write(const bool state) noexcept
{
    const auto pin   = static_cast<gpio_num_t>(myPin);
    const auto level = static_cast<std::uint32_t>(state);
    gpio_set_level(pin, level);   
}

bool Esp32::read() const noexcept
{
    const auto pin = static_cast<gpio_num_t>(myPin);
    return static_cast<bool>(gpio_get_level(pin));
}
} // namespace driver::gpio
