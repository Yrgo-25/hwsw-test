/**
 * @brief Unit tests for the GPIO driver stub.
 */
#include "driver/gpio/stub.h"
#include "yrgo/test/test.h"

#ifdef TESTSUITE

//! @todo Remove this #ifdef in lecture 8 to enable these tests.
#ifdef LECTURE8

namespace driver
{
namespace
{
/**
 * @brief Initialization test.
 *
 *        Verify that the stub is always initialized, and that it reports the direction given at
 *        construction.
 */
TEST(Gpio_Stub, Initialization)
{
    gpio::Stub input{gpio::Direction::Input};
    EXPECT_TRUE(input.isInitialized());
    EXPECT_EQ(static_cast<int>(input.direction()), static_cast<int>(gpio::Direction::Input));

    gpio::Stub output{gpio::Direction::Output};
    EXPECT_TRUE(output.isInitialized());
    EXPECT_EQ(static_cast<int>(output.direction()), static_cast<int>(gpio::Direction::Output));
}

/**
 * @brief Write and read test.
 *
 *        Verify that a value written to the stub can be read back.
 */
TEST(Gpio_Stub, WriteAndRead)
{
    gpio::Stub led{gpio::Direction::Output};
    EXPECT_FALSE(led.read());

    led.write(true);
    EXPECT_TRUE(led.read());

    led.write(false);
    EXPECT_FALSE(led.read());
}

/**
 * @brief Toggle test.
 *
 *        Verify that toggle() inverts the current output.
 */
TEST(Gpio_Stub, Toggle)
{
    gpio::Stub led{gpio::Direction::Output};
    EXPECT_FALSE(led.read());

    led.toggle();
    EXPECT_TRUE(led.read());

    led.toggle();
    EXPECT_FALSE(led.read());
}

/**
 * @brief Simulated input test.
 *
 *        Verify that setState() can be used to simulate an external input, e.g. a button press,
 *        without going through write().
 */
TEST(Gpio_Stub, SimulatedInput)
{
    gpio::Stub button{gpio::Direction::InputPullup};
    EXPECT_FALSE(button.read());

    button.setState(true);
    EXPECT_TRUE(button.read());

    button.setState(false);
    EXPECT_FALSE(button.read());
}

/**
 * @brief Interrupt test.
 *
 *        Verify that pin change interrupt enablement is tracked, both for the GPIO itself and
 *        for its associated I/O port.
 */
TEST(Gpio_Stub, Interrupt)
{
    gpio::Stub button{gpio::Direction::InputPullup};
    EXPECT_FALSE(button.isInterruptEnabled());
    EXPECT_FALSE(button.isPortInterruptEnabled());

    button.enableInterrupt(true);
    EXPECT_TRUE(button.isInterruptEnabled());

    button.enableInterrupt(false);
    EXPECT_FALSE(button.isInterruptEnabled());

    button.enableInterruptOnPort(true);
    EXPECT_TRUE(button.isPortInterruptEnabled());
}
} // namespace
} // namespace driver

#endif /** LECTURE8 */

#endif /** TESTSUITE */
