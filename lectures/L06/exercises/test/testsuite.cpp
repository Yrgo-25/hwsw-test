/**
 * @brief Test suite example.
 */
#include <cstdio>

#include "driver/gpio/esp32.h"

namespace
{
// -----------------------------------------------------------------------------
void printGpioState(const driver::gpio::Esp32& gpio) noexcept
{
    const char* state{gpio.read() ? "high" : "low"};
    std::printf("GPIO state: %s\n", state);
}
} // namespace

/**
 * @brief Application entry point.
 *
 * @return 0 on termination of the program.
 */
int main()
{
    // Create LED connected to pin 9.
    constexpr std::uint8_t led1Pin{9U};
    driver::gpio::Esp32 led1{led1Pin};

    // Print the initial LED state (should be low).
    printGpioState(led1);

    // Enable the LED and print the LED state once more (should be high).
    led1.write(true);
    printGpioState(led1);
    return 0;
}
