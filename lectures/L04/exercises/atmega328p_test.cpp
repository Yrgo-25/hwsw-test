/**
 * @brief Unit tests for the ATmega328p timer driver.
 */
#include <cstdint>

#include "arch/avr/hw_platform.h"
#include "driver/timer/atmega328p.h"
#include "utils/utils.h"
#include "yrgo/test/test.h"

#ifdef TESTSUITE

//! @todo Implement tests according to project requirements.
namespace driver
{
namespace
{
/** Maximum number of timer circuits available on ATmega328P. */
constexpr std::uint8_t MaxTimerCount{3U};

/** Flag to track if callback was invoked. */
bool callbackInvoked{false};

// -----------------------------------------------------------------------------
inline void resetCallbackFlag() noexcept { callbackInvoked = false; }

// -----------------------------------------------------------------------------
inline void testCallback() noexcept { callbackInvoked = true; }

// -----------------------------------------------------------------------------
constexpr std::uint32_t getMaxCount(const std::uint32_t timeout_ms) noexcept
{
    constexpr double interruptIntervalMs{0.128};
    return 0U < timeout_ms ? utils::round<std::uint32_t>(timeout_ms / interruptIntervalMs) : 0U;
}

/**
 * @brief Timer initialization test.
 *
 *        Verify that timer circuits are initialized correctly and that
 *        resource limits are enforced.
 */
TEST(Timer_Atmega328p, Initialization)
{
    constexpr std::uint32_t timeout0_ms{100U};
    constexpr std::uint32_t timeout1_ms{200U};
    constexpr std::uint32_t timeout2_ms{300U};

    // Case 1 - Verify that only MaxTimerCount (3) timers can be used simultaneously due to
    //          hardware limitations.
    {
        // Create MaxTimerCount timers with different timeouts.
        // Verify that each timer is initialized.
        timer::Atmega328p timer0{timeout0_ms};
        timer::Atmega328p timer1{timeout1_ms};
        timer::Atmega328p timer2{timeout2_ms};

        EXPECT_TRUE(timer0.isInitialized());
        EXPECT_TRUE(timer1.isInitialized());
        EXPECT_TRUE(timer2.isInitialized());

        // Create one additional timer.
        // Verify that the additional timer isn't initialized, since no circuits are available.
        timer::Atmega328p timer3{timeout0_ms};
        EXPECT_FALSE(timer3.isInitialized());
    }

    // Case 2 - Verify that a timer cannot have a 0 ms timeout.
    {
        // Create a timer with a 100 ms timeout.
        // Verify that the timer is initialized.
        timer::Atmega328p timer0{timeout0_ms};
        EXPECT_TRUE(timer0.isInitialized());

        //  Create a timer with a 0 ms timeout.
        // Verify that the timer isn't initialized (0 ms is an invalid timeout).
        constexpr std::uint32_t invalidTimeout{0U};
        timer::Atmega328p timer1{invalidTimeout};
        EXPECT_FALSE(timer1.isInitialized());
    }
}

/**
 * @brief Timer enable/disable test.
 *
 *        Verify that timers can be started and stopped correctly.
 */
TEST(Timer_Atmega328p, EnableDisable)
{
    constexpr std::uint32_t timeout_ms{1000U};

    // Create a timer with a timeout.
    timer::Atmega328p timer0{timeout_ms};

    // Verify timer is not enabled initially (unless auto-started via the constructor).
    EXPECT_FALSE(timer0.isEnabled());

    // Start the timer.
    // Verify that the timer is enabled.
    timer0.start();
    EXPECT_TRUE(timer0.isEnabled());

    // Stop the timer.
    // Verify that the timer is disabled.
    timer0.stop();
    EXPECT_FALSE(timer0.isEnabled());

    // Toggle the timer.
    // Verify that the timer is enabled.
    timer0.toggle();
    EXPECT_TRUE(timer0.isEnabled());

    // Toggle the timer once again.
    // Verify that the timer is disabled.
    timer0.toggle();
    EXPECT_FALSE(timer0.isEnabled());
}

/**
 * @brief Timer timeout test.
 *
 *        Verify that timeout values can be set and read correctly.
 */
TEST(Timer_Atmega328p, Timeout)
{
    constexpr std::uint32_t timeout1_ms{100U};
    constexpr std::uint32_t timeout2_ms{200U};
    constexpr std::uint32_t invalidTimeout{0U};

    // Create a timer with an initial timeout of 100 ms.
    // Verify timeout_ms() returns the correct value.
    timer::Atmega328p timer0{timeout1_ms};
    EXPECT_EQ(timer0.timeout_ms(), timeout1_ms);

    // Change the timeout to 200 ms using setTimeout_ms().
    // Verify the new timeout is returned by timeout_ms().
    timer0.setTimeout_ms(timeout2_ms);
    EXPECT_EQ(timer0.timeout_ms(), timeout2_ms);

    // Try to change the timeout to 0 ms using setTimeout_ms().
    // Verify that the timeout is unchanged (0 ms is an invalid timeout).
    timer0.setTimeout_ms(invalidTimeout);
    EXPECT_EQ(timer0.timeout_ms(), timeout2_ms);
}

/**
 * @brief Timer callback test.
 *
 *        Verify that timer callbacks are invoked when timeout occurs.
 */
TEST(Timer_Atmega328p, Callback)
{
    constexpr std::uint32_t timeout_ms{10U};

    // Reset the callback flag (callbackInvoked) using resetCallbackFlag().
    resetCallbackFlag();
    EXPECT_FALSE(callbackInvoked);

    // Create a timer with a short timeout, such as 10 ms, and testCallback() as callback.
    // Start the timer.
    timer::Atmega328p timer0{timeout_ms, testCallback, true};

    // Compute the number of times we need to invoke handleCallback() to generate a timeout.
    const auto maxCount = getMaxCount(timeout_ms);

    // Simulate timer interrupts by repeatedly calling handleCallback() on the timer.
    // Call handleCallback() enough times to reach the timeout (getMaxCount()).
    for (std::uint32_t i{}; i < maxCount; ++i)
    {
        timer0.handleCallback();
    }
    // Verify that callbackInvoked is true after timeout.
    EXPECT_TRUE(callbackInvoked);
}

/**
 * @brief Timer restart test.
 *
 *        Verify that timers can be restarted correctly.
 */
TEST(Timer_Atmega328p, Restart)
{
    constexpr std::uint32_t timeout_ms{100U};

    // Reset the callback flag (callbackInvoked).
    resetCallbackFlag();
    EXPECT_FALSE(callbackInvoked);

    // Create and start a timer with testCallback() as callback.
    timer::Atmega328p timer0{timeout_ms, testCallback, true};

    // Compute the number of times to call handleCallback() to generate a timeout.
    const auto maxCount          = getMaxCount(timeout_ms);
    const auto justBeforeTimeout = maxCount - 1U;

    // Call handleCallback() enough times to almost reach the timeout (getMaxCount() - 1).
    // Verify that the callback flag (callbackInvoked) is still false.
    for (std::uint32_t i{}; i < justBeforeTimeout; ++i)
    {
        timer0.handleCallback();
    }
    EXPECT_FALSE(callbackInvoked);

    // Restart the timer.
    // Verify that the timer is still enabled after restart.
    timer0.restart();
    EXPECT_TRUE(timer0.isEnabled());

    // Call handleCallback() enough times to almost reach the timeout (getMaxCount() - 1).
    // Verify that the callback flag (callbackInvoked) is still false, since the timer was
    // restarted.
    for (std::uint32_t i{}; i < justBeforeTimeout; ++i)
    {
        timer0.handleCallback();
    }
    EXPECT_FALSE(callbackInvoked);

    // Call handleCallback() once more to reach timeout.
    // Verify that the callback flag (callbackInvoked) is true due to timeout.
    timer0.handleCallback();
    EXPECT_TRUE(callbackInvoked);
}
} // namespace
} // namespace driver

#endif /** TESTSUITE */
