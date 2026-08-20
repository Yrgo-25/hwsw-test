/**
 * @file GPIO stub tests.
 */
#include "driver/gpio/stub.h"
#include "yrgo/test/test.h"

/**
 * @brief Test GPIO initial state. 
 *        Verify that the initial state is set to false by default, or true if manually set.
 */
TEST(GpioStub, InitialState)
{
    // Create GPIO instance with initial state unspecified.
    // Expect the GPIO to disabled.
    driver::gpio::Stub gpio1{};
    EXPECT_FALSE(gpio1.read());

    // Create another GPIO instance with initial state set to true.
    // Expect the GPIO to enabled.
    driver::gpio::Stub gpio2{true};
    EXPECT_TRUE(gpio2.read());

    // Create another GPIO instance with initial state set to false.
    // Expect the GPIO to disabled.
    driver::gpio::Stub gpio3{false};
    EXPECT_FALSE(gpio3.read());
}

/**
 * @brief Test GPIO write operations.
 *        Expect the GPIO state to be set as specified.
 */
TEST(GpioStub, Write)
{
    // Create a LED instance.
    driver::gpio::Stub led1{};
    
    // Set LED to true, expect the LED to be enabled.
    led1.write(true);
    EXPECT_TRUE(led1.read());

    // Set LED to false, expect the LED to be disabled.
    led1.write(false);
    EXPECT_FALSE(led1.read());

    // Set LED to true again, expect the LED to be enabled.
    led1.write(true);
    EXPECT_TRUE(led1.read());

    // Set LED to false again, expect the LED to be disabled.
    led1.write(false);
    EXPECT_FALSE(led1.read());
}

/**
 * @brief Test toggle functionality.
 * 
 *        Verify that the GPIO state is toggled correctly.
 */
TEST(GpioStub, Toggle)
{
    constexpr unsigned iterationCount{5U};

    // Create LED instance.
    driver::gpio::Stub led1{};

    // Create variable holding the expected state.
    bool expectedState{led1.read()};

    // Create a loop, toggle the LED five times.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        // Expect the LED state to be flipped on each toggle.
        led1.toggle();
        expectedState = !expectedState;
        EXPECT_EQ(led1.read(), expectedState);
    }
}
