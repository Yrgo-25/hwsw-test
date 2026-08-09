/**
 * @brief Unit tests for the ATmega328p watchdog timer driver.
 */
#include <cstdint>
#include <limits>

#include "yrgo/test/test.h"

#include "arch/avr/hw_platform.h"
#include "driver/watchdog/atmega328p.h"
#include "utils/utils.h"

#ifdef TESTSUITE

namespace driver
{
namespace
{
using Timeout = watchdog::Atmega328p::Timeout;

// -----------------------------------------------------------------------------
watchdog::Interface& initWatchdog() noexcept { return watchdog::Atmega328p::getInstance(); }

// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const std::uint16_t timeout_ms) noexcept
{
    const Timeout timeout{static_cast<Timeout>(timeout_ms)};

    // Ensure the timeout corresponds to a hardware-supported duration.
    switch (timeout)
    {
        case Timeout::Duration16ms:
        case Timeout::Duration32ms:
        case Timeout::Duration64ms:
        case Timeout::Duration128ms:
        case Timeout::Duration256ms:
        case Timeout::Duration512ms:
        case Timeout::Duration1024ms:
        case Timeout::Duration2048ms:
        case Timeout::Duration4096ms:
        case Timeout::Duration8192ms:
            return true;
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
uint8_t mapTimeout(const std::uint16_t timeout_ms) noexcept
{
    const Timeout timeout{static_cast<Timeout>(timeout_ms)};

    // Map timeout to WDTCSR register value.
    switch (timeout)
    {
        case Timeout::Duration16ms:
            return 0U;
        case Timeout::Duration32ms:
            return (1U << WDP0);
        case Timeout::Duration64ms:
            return (1U << WDP1);
        case Timeout::Duration128ms:
            return (1U << WDP1) | (1U << WDP0);
        case Timeout::Duration256ms:
            return (1U << WDP2);
        case Timeout::Duration512ms:
            return (1U << WDP2) | (1U << WDP0);
        case Timeout::Duration1024ms:
            return (1U << WDP2) | (1U << WDP1);
        case Timeout::Duration2048ms:
            return (1U << WDP2) | (1U << WDP1) | (1U << WDP0);
        case Timeout::Duration4096ms:
            return (1U << WDP3);
        case Timeout::Duration8192ms:
            return (1U << WDP3) | (1U << WDP0);
        default:
            return 0xFFU;
    }
}

/**
 * @brief Watchdog timer initialization test.
 *
 *        Verify that the watchdog timer is initialized correctly.
 */
TEST(Watchdog_Atmega328p, Initialization)
{
    // Initialize the watchdog.
    watchdog::Interface& watchdog{initWatchdog()};

    // Expect the watchdog to always be initialized.
    EXPECT_TRUE(watchdog.isInitialized());
}

/**
 * @brief Watchdog timer reset test.
 *
 *        Verify that the watchdog reset flag is cleared after watchdog reset.
 */
TEST(Watchdog_Atmega328p, Reset)
{
    // Initialize and enable the watchdog.
    watchdog::Interface& watchdog{initWatchdog()};
    watchdog.setEnabled(true);

    // Intentionally set the watchdog reset flag, verify that it is set.
    utils::set(MCUSR, WDRF);
    EXPECT_TRUE(utils::read(MCUSR, WDRF));

    // Reset the watchdog, expect the watchdog reset flag to be cleared.
    watchdog.reset();
    EXPECT_FALSE(utils::read(MCUSR, WDRF));
}

/**
 * @brief Watchdog enablement test.
 *
 *        Verify that the watchdog enablement bit is set correctly.
 */
TEST(Watchdog_Atmega328p, Enablement)
{
    // Initialize the watchdog.
    watchdog::Interface& watchdog{initWatchdog()};

    constexpr std::uint8_t toggleCount{10U};
    bool enabled{false};

    // Toggle the watchdog timer a few times.
    // Expect the watchdog enablement bit to be set/cleared accordingly.
    for (std::uint8_t i{}; i < toggleCount; ++i)
    {
        enabled = !enabled;
        watchdog.setEnabled(enabled);
        EXPECT_EQ(utils::read(WDTCSR, WDE), enabled);
        EXPECT_EQ(watchdog.isEnabled(), enabled);
    }
}

/**
 * @brief Watchdog timeout test.
 *
 *        Verify that the only valid watchdog timeouts can be set.
 */
TEST(Watchdog_Atmega328p, Timeout)
{
    // Initialize the watchdog.
    watchdog::Interface& watchdog{initWatchdog()};
    watchdog.setEnabled(true);

    constexpr std::size_t timeoutMax{std::numeric_limits<std::uint16_t>::max()};

    // Attempt different timeouts, expect that only valid values are accepted.
    for (std::size_t timeout_ms{}; timeout_ms <= timeoutMax; ++timeout_ms)
    {
        // Check whether the new timeout is valid.
        const bool timeoutValid{isTimeoutValid(timeout_ms)};

        const std::uint16_t prevTimeout_ms{watchdog.timeout_ms()};
        const std::uint8_t prevPrescaler{WDTCSR};

        // Expect to be able to set the new timeout if valid.
        if (timeoutValid)
        {
            EXPECT_TRUE(watchdog.setTimeout_ms(timeout_ms));

            // Expect the timeout and prescaler bits to be updated accordingly.
            EXPECT_EQ(watchdog.timeout_ms(), timeout_ms);
            EXPECT_EQ(WDTCSR, mapTimeout(timeout_ms));
        }
        else
        {
            // Expect nothing to change if the timeout is invalid.
            EXPECT_FALSE(watchdog.setTimeout_ms(timeout_ms));
            EXPECT_EQ(watchdog.timeout_ms(), prevTimeout_ms);
            EXPECT_EQ(WDTCSR, prevPrescaler);
        }
    }
}
} // namespace
} // namespace driver.

#endif /** TESTSUITE */