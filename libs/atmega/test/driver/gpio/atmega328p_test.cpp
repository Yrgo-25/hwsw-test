/**
 * @brief Unit tests for the Atmega328p GPIO driver.
 */
#include <cstdint>

#include "yrgo/test/test.h"

#include "arch/avr/hw_platform.h"
#include "driver/gpio/atmega328p.h"
#include "utils/utils.h"

#ifdef TESTSUITE

//! @todo Remove this #ifdef in lecture 2 to enable these tests.
#ifdef LECTURE2

namespace driver
{
namespace
{
/**
 * @brief GPIO register structure.
 */
struct GpioRegs
{
    /** Data direction register. */
    volatile std::uint8_t& ddrx;

    /** Port register. */
    volatile std::uint8_t& portx;

    /** Pin register. */
    volatile std::uint8_t& pinx;
};

/**
 * @brief Pin offset structure.
 */
struct PinOffset
{
    /** Pin offset for I/O port D. */
    static constexpr std::uint8_t D{0U};

    /** Pin offset for I/O port B. */
    static constexpr std::uint8_t B{8U};

    /** Pin offset for I/O port C. */
    static constexpr std::uint8_t C{14U};
};

/** Number of available pins. */
constexpr std::uint8_t PinCount{20U};

// -----------------------------------------------------------------------------
constexpr bool isPinValid(const std::uint8_t id) noexcept { return PinCount > id; }

// -----------------------------------------------------------------------------
constexpr std::uint8_t getPhysicalPin(const std::uint8_t id) noexcept
{
    // Return physical pin 0 - 7 on the associated GPIO port.
    if (!isPinValid(id)) { return static_cast<std::uint8_t>(-1); }
    if (PinOffset::B > id) { return id; }
    else if (PinOffset::C > id) { return id - PinOffset::B; }
    return id - PinOffset::C;
}

// -----------------------------------------------------------------------------
constexpr void simulateToggle(GpioRegs& regs) noexcept
{
    constexpr std::uint8_t bitCount{8U};

    // Check each pin one by one.
    for (std::uint8_t pin{}; pin < bitCount; ++pin)
    {
        // Toggle the output of a given pin if configured as output and the pin bit has been set.
        if (utils::read(regs.ddrx, pin) && utils::read(regs.pinx, pin))
        {
            utils::toggle(regs.portx, pin);
            utils::clear(regs.pinx, pin);
        }
    }
}

// -----------------------------------------------------------------------------
void runOutputTest(const std::uint8_t id, GpioRegs& regs)
{
    // Get the physical pin on the given port.
    // Example: const std::uint8_t pin{getPhysicalPin(id)};

    // Limit the scope of the GPIO instance.
    {
        // Create a new GPIO output.

        // Expect the instance to be initialized correctly if the pin is valid.

        // Expect the GPIO to be set as output, i.e., the corresponding bit in DDRx should be set.
        // Tips: Check that the pin is set with EXPECT_TRUE() and utils::read(regs.ddrx, pin).

        // Set the output high, expect the corresponding bit in PORTx to be set.
        // Tips: Use gpio.write() to set the output, read the bit with
        // utils::read(regs.portx, pin);

        // Set the output low, expect the corresponding bit in PORTx to be cleared.

        // Toggle the output, expect the corresponding bit in PORTx to be set.

        // Toggle the output again, expect the corresponding bit in PORTx to be cleared.

        // Toggle the output once more, expect the corresponding bit in PORTx to be set.
    }
    // Expect DDRx and PORTx to be cleared after the instance has been deleted.
}

// -----------------------------------------------------------------------------
void runInputTest(const std::uint8_t id, GpioRegs& regs)
{
    // Get the physical pin on the given port.

    // Limit the scope of the GPIO instance.
    {
        // Create a new GPIO input with internal pull-up resistor enabled.
        // Expect the instance to be initialized correctly if the pin is valid.

        // Expect the GPIO to be set as input, i.e., the corresponding bit in DDRx should be
        // cleared.

        // Expect the internal pull-up resistor to be enabled, i.e., the corresponding bit in PORTx
        // should be set.

        // Set the input high in PINx, expect the GPIO input to be high.

        // Set the input low in PINx, expect the GPIO input to be low.
    }
    // Expect DDRx and PORTx to be cleared after the instance has been deleted.
}

/**
 * @brief GPIO initialization test.
 *
 *        Verify that only one instance per valid pin can be used at once.
 */
TEST(Gpio_Atmega328p, Initialization)
{
    constexpr std::uint8_t pinMax{50U};

    // Systematically test GPIO initialization across a range of pin numbers.
    for (std::uint8_t pin{}; pin < pinMax; ++pin)
    {
        // Create a new GPIO instance with the current pin number.
        // Example: gpio::Atmega328p gpio{pin, gpio::Mode::Output};

        // Expect the instance to be initialized correctly if the pin is valid.
        // Tips: Check if the instance is initialized by invoking gpio.isInitialized().
        //       Check if the pin is valid by invoking isPinValid(pin).
        //       Use EXPECT_TRUE(), EXPECT_FALSE, and/or EXPECT_EQ to validate the functionality.

        // Create another GPIO instance on the same pin.
        // Expect the instance to not be initialized, since the pin is already reserved.
    }
}

/**
 * @brief GPIO output test.
 *
 *        Verify that GPIO outputs can be used for reading and writing.
 */
TEST(Gpio_Atmega328p, Output)
{
    // Systematically test I/O port D.
    for (std::uint8_t pin{}; pin < PinOffset::B; ++pin)
    {
        GpioRegs regs{DDRD, PORTD, PIND};
        runOutputTest(pin, regs);
    }

    // Systematically test I/O port B.
    for (std::uint8_t pin{PinOffset::B}; pin < PinOffset::C; ++pin)
    {
        GpioRegs regs{DDRB, PORTB, PINB};
        runOutputTest(pin, regs);
    }

    // Systematically test I/O port C.
    for (std::uint8_t pin{PinOffset::C}; pin < PinCount; ++pin)
    {
        GpioRegs regs{DDRC, PORTC, PINC};
        runOutputTest(pin, regs);
    }
}

/**
 * @brief GPIO input test.
 *
 *        Verify that GPIO input can be used for reading.
 */
TEST(Gpio_Atmega328p, Input)
{
    // Systematically test I/O port D.
    for (std::uint8_t pin{}; pin < PinOffset::B; ++pin)
    {
        GpioRegs regs{DDRD, PORTD, PIND};
        runInputTest(pin, regs);
    }

    // Systematically test I/O port B.
    for (std::uint8_t pin{PinOffset::B}; pin < PinOffset::C; ++pin)
    {
        GpioRegs regs{DDRB, PORTB, PINB};
        runInputTest(pin, regs);
    }

    // Systematically test I/O port C.
    for (std::uint8_t pin{PinOffset::C}; pin < PinCount; ++pin)
    {
        GpioRegs regs{DDRC, PORTC, PINC};
        runInputTest(pin, regs);
    }
}
} // namespace
} // namespace driver

//! @todo Remove this #endif in lecture 2 to enable these tests.
#endif /** LECTURE2 */

#endif /** TESTSUITE */
