/**
 * @file GPIO stub tests.
 */
#include "driver/gpio/interface.h"
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

/**
 * @brief Test toggling from the enabled state.
 *
 *        Verify that toggling works in both directions, also when the GPIO starts out enabled.
 */
TEST(GpioStub, ToggleFromEnabledState)
{
    // Create an enabled GPIO instance.
    driver::gpio::Stub led1{true};
    EXPECT_TRUE(led1.read());

    // Toggle the LED, expect it to be disabled.
    led1.toggle();
    EXPECT_FALSE(led1.read());

    // Toggle the LED again, expect it to be enabled.
    led1.toggle();
    EXPECT_TRUE(led1.read());
}

/**
 * @brief Test repeated writes of the same state.
 *
 *        Verify that writing the same state several times keeps the GPIO in that state, rather
 *        than behaving like a toggle.
 */
TEST(GpioStub, RepeatedWriteKeepsState)
{
    constexpr unsigned iterationCount{3U};

    // Create a LED instance.
    driver::gpio::Stub led1{};

    // Write true several times, expect the LED to stay enabled.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        led1.write(true);
        EXPECT_TRUE(led1.read());
    }

    // Write false several times, expect the LED to stay disabled.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        led1.write(false);
        EXPECT_FALSE(led1.read());
    }
}

/**
 * @brief Test writing after toggling.
 *
 *        Verify that a write overrides the state set by a preceding toggle.
 */
TEST(GpioStub, WriteAfterToggle)
{
    // Create a LED instance, toggle it so that it becomes enabled.
    driver::gpio::Stub led1{};
    led1.toggle();
    EXPECT_TRUE(led1.read());

    // Write false, expect the write to override the toggled state.
    led1.write(false);
    EXPECT_FALSE(led1.read());

    // Toggle again, expect the toggle to continue from the written state.
    led1.toggle();
    EXPECT_TRUE(led1.read());
}

/**
 * @brief Test that GPIO instances are independent.
 *
 *        Verify that operations on one GPIO do not affect another.
 */
TEST(GpioStub, InstancesAreIndependent)
{
    // Create two LED instances with opposite initial states.
    driver::gpio::Stub led1{false};
    driver::gpio::Stub led2{true};

    // Enable the first LED, expect the second one to be unaffected.
    led1.write(true);
    EXPECT_TRUE(led1.read());
    EXPECT_TRUE(led2.read());

    // Toggle the second LED, expect the first one to be unaffected.
    led2.toggle();
    EXPECT_TRUE(led1.read());
    EXPECT_FALSE(led2.read());
}

/**
 * @brief Test using the GPIO through the driver interface.
 *
 *        Verify that the stub is usable via a driver::gpio::Interface reference, which is how
 *        application code is expected to consume it.
 */
TEST(GpioStub, UsageViaInterface)
{
    // Create a LED instance and access it through the driver interface.
    driver::gpio::Stub stub{};
    driver::gpio::Interface& led1{stub};

    // Expect the LED to be disabled by default.
    EXPECT_FALSE(led1.read());

    // Enable the LED through the interface, expect it to be enabled.
    led1.write(true);
    EXPECT_TRUE(led1.read());

    // Toggle the LED through the interface, expect it to be disabled.
    led1.toggle();
    EXPECT_FALSE(led1.read());

    // Expect the change to be visible on the underlying stub as well.
    EXPECT_FALSE(stub.read());
}
