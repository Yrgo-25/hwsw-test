/**
 * @file Timer stub tests.
 */
#include <cstdint>

#include "driver/timer/interface.h"
#include "driver/timer/stub.h"
#include "yrgo/test/test.h"

namespace
{
// -----------------------------------------------------------------------------
void tickTimes(driver::timer::Interface& timer, const std::uint32_t count) noexcept
{
    for (std::uint32_t i{}; i < count; ++i)
    {
        timer.tick();
    }
}
} // namespace

/**
 * @brief Test the initial state of a timer.
 *
 *        Verify that the configured timeout is reported back, and that the timer is stopped and
 *        not timed out directly after construction.
 */
TEST(TimerStub, InitialState)
{
    constexpr std::uint16_t timeout_ms{100U};

    // Create a timer without specifying whether to start it.
    driver::timer::Stub timer{timeout_ms};

    // Expect the configured timeout to be reported back unchanged.
    EXPECT_EQ(timer.timeout_ms(), timeout_ms);

    // Expect the timer to be stopped and not timed out.
    EXPECT_FALSE(timer.isRunning());
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Test construction with the timer started immediately.
 *
 *        Verify that the second constructor parameter controls whether the timer is running
 *        directly after construction.
 */
TEST(TimerStub, InitialStateWhenStarted)
{
    constexpr std::uint16_t timeout_ms{100U};

    // Create a timer that is started immediately, expect it to be running.
    driver::timer::Stub timer1{timeout_ms, true};
    EXPECT_TRUE(timer1.isRunning());

    // Create a timer that is explicitly not started, expect it to be stopped.
    driver::timer::Stub timer2{timeout_ms, false};
    EXPECT_FALSE(timer2.isRunning());
}

/**
 * @brief Test starting and stopping the timer.
 *
 *        Verify that start() puts the timer in the running state and that stop() takes it out
 *        of it.
 */
TEST(TimerStub, StartAndStop)
{
    // Create a stopped timer.
    driver::timer::Stub timer{100U};
    EXPECT_FALSE(timer.isRunning());

    // Start the timer, expect it to be running.
    timer.start();
    EXPECT_TRUE(timer.isRunning());

    // Stop the timer, expect it to be stopped.
    timer.stop();
    EXPECT_FALSE(timer.isRunning());

    // Start the timer again, expect it to be running.
    timer.start();
    EXPECT_TRUE(timer.isRunning());
}

/**
 * @brief Test toggling the timer.
 *
 *        Verify that the running state is flipped on each toggle.
 */
TEST(TimerStub, Toggle)
{
    constexpr unsigned iterationCount{5U};

    // Create a stopped timer.
    driver::timer::Stub timer{100U};

    // Create a variable holding the expected running state.
    bool expectedState{timer.isRunning()};

    // Toggle the timer five times, expect the running state to be flipped each time.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        timer.toggle();
        expectedState = !expectedState;
        EXPECT_EQ(timer.isRunning(), expectedState);
    }
}

/**
 * @brief Test that a stopped timer does not advance.
 *
 *        Verify that tick() has no effect while the timer is stopped, no matter how many times
 *        it is called.
 */
TEST(TimerStub, TickWhileStopped)
{
    constexpr std::uint16_t timeout_ms{5U};

    // Create a stopped timer.
    driver::timer::Stub timer{timeout_ms};

    // Tick well past the configured timeout while stopped.
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms * 4U));

    // Expect the timer to still be stopped and to never have timed out.
    EXPECT_FALSE(timer.isRunning());
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Test that the timer does not time out too early.
 *
 *        Verify that no timeout is reported until the configured number of ticks has elapsed.
 */
TEST(TimerStub, NoTimeoutBeforeConfiguredTicks)
{
    constexpr std::uint16_t timeout_ms{10U};

    // Create a running timer.
    driver::timer::Stub timer{timeout_ms, true};

    // Tick one time less than the configured timeout, expect no timeout.
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Test that the timer times out after the configured number of ticks.
 *
 *        Verify that a timeout is reported once the configured number of ticks has elapsed.
 */
TEST(TimerStub, TimeoutAfterConfiguredTicks)
{
    constexpr std::uint16_t timeout_ms{10U};

    // Create a running timer.
    driver::timer::Stub timer{timeout_ms, true};

    // Tick exactly the configured timeout, expect a timeout.
    tickTimes(timer, timeout_ms);
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test that reading a timeout clears it.
 *
 *        Verify that hasTimedOut() clears the elapsed time, so that a timeout is reported once
 *        only and is not repeated on the next read.
 */
TEST(TimerStub, TimeoutIsClearedWhenRead)
{
    constexpr std::uint16_t timeout_ms{4U};

    // Create a running timer and tick it up to the timeout.
    driver::timer::Stub timer{timeout_ms, true};
    tickTimes(timer, timeout_ms);

    // Expect the first read to report the timeout.
    EXPECT_TRUE(timer.hasTimedOut());

    // Expect the second read to report no timeout, since the first read cleared it.
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Test that the timer times out periodically.
 *
 *        Verify that the timer keeps timing out every configured number of ticks, without having
 *        to be restarted in between.
 */
TEST(TimerStub, TimeoutIsPeriodic)
{
    constexpr std::uint16_t timeout_ms{4U};
    constexpr unsigned iterationCount{3U};

    // Create a running timer.
    driver::timer::Stub timer{timeout_ms, true};

    // Expect a timeout on every configured number of ticks.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        // Expect no timeout just before the configured number of ticks has elapsed.
        tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));
        EXPECT_FALSE(timer.hasTimedOut());

        // Expect a timeout once the configured number of ticks has elapsed.
        timer.tick();
        EXPECT_TRUE(timer.hasTimedOut());
    }
}

/**
 * @brief Test that starting the timer clears the elapsed time.
 *
 *        Verify that start() restarts the measurement from zero, so that a partially elapsed
 *        timeout is discarded.
 */
TEST(TimerStub, StartClearsElapsedTime)
{
    constexpr std::uint16_t timeout_ms{5U};

    // Create a running timer and tick it to just before the timeout.
    driver::timer::Stub timer{timeout_ms, true};
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));

    // Restart the timer, which is expected to discard the elapsed time.
    timer.start();

    // Tick to just before the timeout again, expect no timeout yet.
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));
    EXPECT_FALSE(timer.hasTimedOut());

    // Tick one more time, expect a timeout.
    timer.tick();
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test that toggling the timer clears the elapsed time.
 *
 *        Verify that toggle() discards a partially elapsed timeout, just like start() does.
 */
TEST(TimerStub, ToggleClearsElapsedTime)
{
    constexpr std::uint16_t timeout_ms{5U};

    // Create a running timer and tick it to just before the timeout.
    driver::timer::Stub timer{timeout_ms, true};
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));

    // Toggle the timer off and on again, which is expected to discard the elapsed time.
    timer.toggle();
    EXPECT_FALSE(timer.isRunning());
    timer.toggle();
    EXPECT_TRUE(timer.isRunning());

    // Tick to just before the timeout again, expect no timeout yet.
    tickTimes(timer, static_cast<std::uint16_t>(timeout_ms - 1U));
    EXPECT_FALSE(timer.hasTimedOut());

    // Tick one more time, expect a timeout.
    timer.tick();
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test that the elapsed time does not run past the timeout.
 *
 *        Verify that the elapsed time stops at the timeout instead of running on. Ticking a full
 *        range of the underlying 16-bit counter must not wrap it back below the timeout, which
 *        would make an elapsed timeout disappear.
 */
TEST(TimerStub, ElapsedTimeSaturatesAtTimeout)
{
    constexpr std::uint16_t timeout_ms{3U};

    // Number of ticks needed to wrap a 16-bit counter back to zero.
    constexpr std::uint32_t counterRange{65536U};

    // Create a running timer and tick it a full counter range past the configured timeout.
    driver::timer::Stub timer{timeout_ms, true};
    tickTimes(timer, counterRange);

    // Expect the timeout to still be reported, i.e. the elapsed time saturated instead of
    // wrapping around.
    EXPECT_TRUE(timer.hasTimedOut());

    // Expect no further timeout, since reading the timeout cleared the elapsed time.
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Test that stopping the timer keeps an already reached timeout.
 *
 *        Verify that stop() does not discard the elapsed time, so a timeout reached before the
 *        timer was stopped is still reported.
 */
TEST(TimerStub, StopKeepsReachedTimeout)
{
    constexpr std::uint16_t timeout_ms{3U};

    // Create a running timer and tick it up to the timeout.
    driver::timer::Stub timer{timeout_ms, true};
    tickTimes(timer, timeout_ms);

    // Stop the timer, expect the already reached timeout to remain.
    timer.stop();
    EXPECT_FALSE(timer.isRunning());
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test the smallest permitted timeout.
 *
 *        Verify that a timer configured with a timeout of one millisecond times out on a single
 *        tick.
 */
TEST(TimerStub, MinimumTimeout)
{
    constexpr std::uint16_t timeout_ms{1U};

    // Create a running timer with the smallest permitted timeout.
    driver::timer::Stub timer{timeout_ms, true};
    EXPECT_EQ(timer.timeout_ms(), timeout_ms);

    // Expect no timeout before the first tick.
    EXPECT_FALSE(timer.hasTimedOut());

    // Expect a timeout after a single tick.
    timer.tick();
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test that timer instances are independent.
 *
 *        Verify that operations on one timer do not affect another.
 */
TEST(TimerStub, InstancesAreIndependent)
{
    constexpr std::uint16_t timeout1_ms{2U};
    constexpr std::uint16_t timeout2_ms{50U};

    // Create two running timers with different timeouts.
    driver::timer::Stub timer1{timeout1_ms, true};
    driver::timer::Stub timer2{timeout2_ms, true};

    // Tick the first timer up to its timeout.
    tickTimes(timer1, timeout1_ms);

    // Expect the first timer to have timed out, but not the second.
    EXPECT_TRUE(timer1.hasTimedOut());
    EXPECT_FALSE(timer2.hasTimedOut());

    // Expect each timer to still report its own timeout.
    EXPECT_EQ(timer1.timeout_ms(), timeout1_ms);
    EXPECT_EQ(timer2.timeout_ms(), timeout2_ms);

    // Stop the first timer, expect the second one to keep running.
    timer1.stop();
    EXPECT_FALSE(timer1.isRunning());
    EXPECT_TRUE(timer2.isRunning());
}

/**
 * @brief Test using the timer through the driver interface.
 *
 *        Verify that the stub is usable via a driver::timer::Interface reference, which is how
 *        application code is expected to consume it.
 */
TEST(TimerStub, UsageViaInterface)
{
    constexpr std::uint16_t timeout_ms{3U};

    // Create a stopped timer and access it through the driver interface.
    driver::timer::Stub stub{timeout_ms};
    driver::timer::Interface& timer{stub};

    // Expect the configured timeout to be reported back through the interface.
    EXPECT_EQ(timer.timeout_ms(), timeout_ms);
    EXPECT_FALSE(timer.isRunning());

    // Start the timer through the interface, expect it to be running.
    timer.start();
    EXPECT_TRUE(timer.isRunning());

    // Tick up to the timeout through the interface, expect a timeout.
    tickTimes(timer, timeout_ms);
    EXPECT_TRUE(timer.hasTimedOut());
}
