/**
 * @brief Unit tests for the timer driver stub.
 */
#include <cstdint>

#include "yrgo/test/test.h"

#include "driver/timer/stub.h"

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
 *        Verify that the stub is always initialized, disabled and not timed out at
 *        construction, and that it reports the timeout given at construction.
 */
TEST(Timer_Stub, Initialization)
{
    constexpr std::uint32_t timeout_ms{100U};
    timer::Stub timer{timeout_ms};

    EXPECT_TRUE(timer.isInitialized());
    EXPECT_FALSE(timer.isEnabled());
    EXPECT_FALSE(timer.hasTimedOut());
    EXPECT_EQ(timer.timeout_ms(), timeout_ms);
}

/**
 * @brief Start and stop test.
 *
 *        Verify that start() enables the timer and that stop() disables it.
 */
TEST(Timer_Stub, StartAndStop)
{
    timer::Stub timer{100U};
    EXPECT_FALSE(timer.isEnabled());

    timer.start();
    EXPECT_TRUE(timer.isEnabled());

    timer.stop();
    EXPECT_FALSE(timer.isEnabled());
}

/**
 * @brief Toggle test.
 *
 *        Verify that toggle() inverts the enablement of the timer.
 */
TEST(Timer_Stub, Toggle)
{
    timer::Stub timer{100U};
    EXPECT_FALSE(timer.isEnabled());

    timer.toggle();
    EXPECT_TRUE(timer.isEnabled());

    timer.toggle();
    EXPECT_FALSE(timer.isEnabled());
}

/**
 * @brief Simulated timeout test.
 *
 *        Verify that setTimedOut() can be used to simulate a timeout, without having to wait for
 *        real, elapsed time.
 */
TEST(Timer_Stub, SimulatedTimeout)
{
    timer::Stub timer{100U};
    timer.start();
    EXPECT_FALSE(timer.hasTimedOut());

    timer.setTimedOut(true);
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Restart test.
 *
 *        Verify that restart() enables the timer and clears a simulated timeout.
 */
TEST(Timer_Stub, Restart)
{
    timer::Stub timer{100U};
    timer.setTimedOut(true);
    timer.stop();

    timer.restart();
    EXPECT_TRUE(timer.isEnabled());
    EXPECT_FALSE(timer.hasTimedOut());
}

/**
 * @brief Set timeout test.
 *
 *        Verify that setTimeout_ms() updates the timeout reported by timeout_ms().
 */
TEST(Timer_Stub, SetTimeout)
{
    timer::Stub timer{100U};
    timer.setTimeout_ms(250U);
    EXPECT_EQ(timer.timeout_ms(), 250U);
}
} // namespace
} // namespace driver

#endif /** LECTURE8 */

#endif /** TESTSUITE */
