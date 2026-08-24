/**
 * @brief Unit tests for the Atmega328p GPIO driver.
 */
#include <cstdint>

#include "arch/avr/hw_platform.h"
#include "driver/gpio/atmega328p.h"
#include "utils/utils.h"
#include "yrgo/test/test.h"

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
[[nodiscard]] constexpr bool isPinValid(const std::uint8_t pin) noexcept { return PinCount > pin; }

// -----------------------------------------------------------------------------
[[nodiscard]] constexpr std::uint8_t getRegBit(const std::uint8_t pin) noexcept
{
    // Return register bit 0 - 7 on the associated GPIO port.
    if (!isPinValid(pin)) { return static_cast<std::uint8_t>(-1); }
    if (PinOffset::B > pin) { return pin; }
    else if (PinOffset::C > pin) { return pin - PinOffset::B; }
    return pin - PinOffset::C;
}

// -----------------------------------------------------------------------------
constexpr void simulateToggle(GpioRegs& regs) noexcept
{
    constexpr std::uint8_t bitCount{8U};

    // Check each register bit one by one.
    for (std::uint8_t bit{}; bit < bitCount; ++bit)
    {
        // Toggle the output of a given bit if configured as output and the bit in PINx is set.
        if (utils::read(regs.ddrx, bit) && utils::read(regs.pinx, bit))
        {
            utils::toggle(regs.portx, bit);
            utils::clear(regs.pinx, bit);
        }
    }
}

// -----------------------------------------------------------------------------
void runOutputTest(const std::uint8_t pin, GpioRegs& regs)
{
    // Get the associated register bit on the given port by invoking getRegBit().

    // Limit the scope of the GPIO instance.
    {
        // Create a new GPIO output.

        // Expect the instance to be initialized correctly if the pin is valid.

        // Expect the GPIO to be set as output, i.e., the corresponding bit in DDRx should be set.
        // Tips: Check that the pin is set with EXPECT_TRUE() and utils::read(regs.ddrx, bit).

        // Set the output high, expect the corresponding bit in PORTx to be set.
        // Tip: Use gpio.write() to set the output, read the bit with utils::read(regs.portx, bit);

        // Set the output low, expect the corresponding bit in PORTx to be cleared.

        // Toggle the output, expect the corresponding bit in PORTx to be set.

        // Toggle the output again, expect the corresponding bit in PORTx to be cleared.

        // Toggle the output once more, expect the corresponding bit in PORTx to be set.
    }
    // Expect DDRx and PORTx to be cleared after the instance has been deleted.
}

// -----------------------------------------------------------------------------
void runInputTest(const std::uint8_t pin, GpioRegs& regs)
{
    // Get the associated register bit on the given port.

    // Limit the scope of the GPIO instance.
    {
        // Create a new GPIO input without its internal pull-up resistor disabled.
        // Expect the instance to be initialized correctly if the pin is valid.

        // Expect the GPIO to be set as input, i.e., the corresponding bit in DDRx should be
        // cleared.

        // Expect the internal pull-up resistor to be disabled, i.e., the corresponding bit in
        // PORTx should be cleared.

        // Set the input high in PINx, expect the GPIO input to be high.

        // Set the input low in PINx, expect the GPIO input to be low.
    }
    // Expect DDRx and PORTx to be cleared after the instance has been deleted.
}

// -----------------------------------------------------------------------------
void runInputPullupTest(const std::uint8_t pin, GpioRegs& regs)
{
    // Get the associated register bit on the given port.

    // Limit the scope of the GPIO instance.
    {
        // Create a new GPIO input with its internal pull-up resistor enabled.
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
        // Create a new GPIO instance with the current pin number, use any valid mode.

        // Expect the instance to be initialized correctly if the pin is valid.

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

/**
 * @brief GPIO input test with internal pull-up resistors enabled.
 *
 *        Verify that GPIO inputs can be used for reading, and that the internal pull-up resistor
 *        is enabled via PORTx when the GPIO is configured as input with pull-up.
 */
TEST(Gpio_Atmega328p, InputPullup)
{
    // Systematically test I/O port D.
    for (std::uint8_t pin{}; pin < PinOffset::B; ++pin)
    {
        GpioRegs regs{DDRD, PORTD, PIND};
        runInputPullupTest(pin, regs);
    }

    // Systematically test I/O port B.
    for (std::uint8_t pin{PinOffset::B}; pin < PinOffset::C; ++pin)
    {
        GpioRegs regs{DDRB, PORTB, PINB};
        runInputPullupTest(pin, regs);
    }

    // Systematically test I/O port C.
    for (std::uint8_t pin{PinOffset::C}; pin < PinCount; ++pin)
    {
        GpioRegs regs{DDRC, PORTC, PINC};
        runInputPullupTest(pin, regs);
    }
}
} // namespace
} // namespace driver

//! @todo Remove this #endif in lecture 2 to enable these tests.
#endif /** LECTURE2 */

#endif /** TESTSUITE */
