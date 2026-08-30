/**
 * @brief Unit tests for the ATmega328p timer driver.
 */
#include <cstdint>

#include "arch/avr/hw_platform.h"
#include "driver/timer/atmega328p.h"
#include "utils/utils.h"
#include "yrgo/test/test.h"

#ifdef TESTSUITE

//! @todo Remove this #ifdef in lecture 4 to enable these tests.
#ifdef LECTURE4

//! @todo Implement tests according to project requirements.
namespace driver
{
namespace
{
/** Maximum number of timer circuits available on ATmega328P. */
constexpr std::uint8_t MaxTimerCount{3U};

//! @todo Remove this #ifdef when starting to work on the callback test.
#ifdef CALLBACK

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

//! @todo Remove this #endif when starting to work on the callback test.
#endif /** CALLBACK */

/**
 * @brief Timer initialization test.
 *
 *        Verify that timer circuits are initialized correctly and that
 *        resource limits are enforced.
 */
TEST(Timer_Atmega328p, Initialization)
{
    // Case 1 - Verify that only MaxTimerCount (3) timers can be used simultaneously due to
    //          hardware limitations.
    {
        //! @todo Test timer initialization:
        // Create MaxTimerCount timers with different timeouts.
        // Verify that each timer is initialized.
        // Create one additional timer.
        // Verify that the additional timer isn't initialized, since no circuits are available.
    }

    // Case 2 - Verify that a timer cannot have a 0 ms timeout.
    {
        // Create a timer with a 100 ms timeout.
        // Verify that the timer is initialized.

        //  Create a timer with a 0 ms timeout.
        // Verify that the timer isn't initialized (0 ms is an invalid timeout).
    }
}

/**
 * @brief Timer enable/disable test.
 *
 *        Verify that timers can be started and stopped correctly.
 */
TEST(Timer_Atmega328p, EnableDisable)
{
    //! @todo Test timer enablement.
    // Create a timer with a timeout.
    // Verify timer is not enabled initially (unless auto-started via the constructor).
    // Start the timer.
    // Verify that the timer is enabled.
    // Stop the timer.
    // Verify that the timer is disabled.
    // Toggle the timer.
    // Verify that the timer is enabled.
    // Toggle the timer once again.
    // Verify that the timer is disabled.

    //! @note Once the above is working:
    //!       Feel free to try all three timers. When enabling/disabling, feel free to check both
    //!       that the isEnabled() methods returns the right value and that the associated bit
    //!       in the timer mask register is set (see the source code).
    //!       Feel free to add a function and pass the timer, the mask register and the mask bit
    //!       to avoid writing the same code three times (or use a struct as was the case for
    //!       the registers in the GPIO unit tests).
}

/**
 * @brief Timer timeout test.
 *
 *        Verify that timeout values can be set and read correctly.
 */
TEST(Timer_Atmega328p, Timeout)
{
    //! @todo Test timer timeout:
    // Create a timer with an initial timeout of 100 ms.
    // Verify timeout_ms() returns the correct value.
    // Change the timeout to 200 ms using setTimeout_ms().
    // Verify the new timeout is returned by timeout_ms().
    // Change the timeout to 0 ms using setTimeout_ms().
    // Verify that the timeout is unchanged (0 ms is an invalid timeout).
}

/**
 * @brief Timer callback test.
 *
 *        Verify that timer callbacks are invoked when timeout occurs.
 */
TEST(Timer_Atmega328p, Callback)
{
    //! @todo Test timer callback:
    // Reset the callback flag (callbackInvoked) using resetCallbackFlag().
    // Create a timer with a short timeout, such as 10 ms, and testCallback() as callback.
    // Start the timer.
    // Simulate timer interrupts by repeatedly calling handleCallback() on the timer.
    // Call handleCallback() enough times to reach the timeout (getMaxCount()).
    // Verify that callbackInvoked is true after timeout.
    // Note: handleCallback() increments the timer and invokes the callback when timeout is reached.
}

/**
 * @brief Timer restart test.
 *
 *        Verify that timers can be restarted correctly.
 */
TEST(Timer_Atmega328p, Restart)
{
    //! @todo Test timer restart:
    // Reset the callback flag (callbackInvoked).
    // Create and start a timer with testCallback() as callback.
    // Call handleCallback() enough times to almost reach the timeout (getMaxCount() - 1).
    // Verify that the callback flag (callbackInvoked) is still false.
    // Restart the timer.
    // Verify that the timer is still enabled after restart.
    // Call handleCallback() enough times to almost reach the timeout (getMaxCount() - 1).
    // Verify that the callback flag (callbackInvoked) is still false, since the timer was
    // restarted. Call handleCallback() again to reach timeout. Verify that the callback flag
    // (callbackInvoked) is true due to timeout.
}

//! @todo Add more tests here (e.g., register verification, multiple timers running simultaneously).

} // namespace
} // namespace driver

//! @todo Remove this #endif in lecture 4 to enable these tests.
#endif /** LECTURE4 */

#endif /** TESTSUITE */
