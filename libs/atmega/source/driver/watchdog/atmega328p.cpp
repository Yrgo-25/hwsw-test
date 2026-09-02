/**
 * @brief Watchdog timer driver implementation details for ATmega328P.
 */
#include <stdint.h>

#include "arch/avr/hw_platform.h"
#include "driver/watchdog/atmega328p.h"
#include "utils/utils.h"

namespace driver
{
namespace watchdog
{
namespace
{
/** Default watchdog timeout (1024 ms). */
constexpr Atmega328p::Timeout DefaultTimeout{Atmega328p::Timeout::Duration1024ms};

// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const Atmega328p::Timeout timeout) noexcept
{
    // Ensure the timeout corresponds to a hardware-supported duration.
    switch (timeout)
    {
        case Atmega328p::Timeout::Duration16ms:
        case Atmega328p::Timeout::Duration32ms:
        case Atmega328p::Timeout::Duration64ms:
        case Atmega328p::Timeout::Duration128ms:
        case Atmega328p::Timeout::Duration256ms:
        case Atmega328p::Timeout::Duration512ms:
        case Atmega328p::Timeout::Duration1024ms:
        case Atmega328p::Timeout::Duration2048ms:
        case Atmega328p::Timeout::Duration4096ms:
        case Atmega328p::Timeout::Duration8192ms:
            return true;
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
uint8_t mapTimeout(const Atmega328p::Timeout timeout) noexcept
{
    // Map timeout to WDTCSR register value.
    switch (timeout)
    {
        case Atmega328p::Timeout::Duration16ms:
            return 0U;
        case Atmega328p::Timeout::Duration32ms:
            return (1U << WDP0);
        case Atmega328p::Timeout::Duration64ms:
            return (1U << WDP1);
        case Atmega328p::Timeout::Duration128ms:
            return (1U << WDP1) | (1U << WDP0);
        case Atmega328p::Timeout::Duration256ms:
            return (1U << WDP2);
        case Atmega328p::Timeout::Duration512ms:
            return (1U << WDP2) | (1U << WDP0);
        case Atmega328p::Timeout::Duration1024ms:
            return (1U << WDP2) | (1U << WDP1);
        case Atmega328p::Timeout::Duration2048ms:
            return (1U << WDP2) | (1U << WDP1) | (1U << WDP0);
        case Atmega328p::Timeout::Duration4096ms:
            return (1U << WDP3);
        case Atmega328p::Timeout::Duration8192ms:
            return (1U << WDP3) | (1U << WDP0);
        default:
            return 0xFFU;
    }
}
} // namespace

// -----------------------------------------------------------------------------
Interface& Atmega328p::getInstance() noexcept
{
    // Create and initialize the singleton watchdog timer instance (once only).
    static Atmega328p myInstance{};

    // Return a reference to the singleton watchdog instance, cast to the corresponding interface.
    return myInstance;
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Atmega328p::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Atmega328p::setEnabled(const bool enable) noexcept
{
    // Reset the watchdog to prevent a timeout during the enablement update.
    reset();

    // Update the enablement status, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    utils::set(WDTCSR, WDCE, WDE);
    if (enable) { utils::set(WDTCSR, WDE); }
    else { utils::clear(WDTCSR, WDE); }

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();

    // Update the enablement state.
    myEnabled = enable;
}

// -----------------------------------------------------------------------------
uint16_t Atmega328p::timeout_ms() const noexcept { return static_cast<uint16_t>(myTimeout); }

// -----------------------------------------------------------------------------
bool Atmega328p::setTimeout_ms(const uint16_t timeout_ms) noexcept
{
    // Attempt to set new timeout, return true on success.
    return setTimeout(static_cast<Timeout>(timeout_ms));
}

// -----------------------------------------------------------------------------
void Atmega328p::reset() noexcept
{
    // Disable interrupts during the reset process.
    utils::globalInterruptDisable();

    // Reset the watchdog and clear the corresponding reset flag.
    asm("WDR");
    utils::clear(MCUSR, WDRF);

    // Re-enable interrupts once the reset process is complete.
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p() noexcept
    : myTimeout{}
    , myEnabled{false}
{
    // Set the default timeout.
    setTimeout(DefaultTimeout);
}

// -----------------------------------------------------------------------------
bool Atmega328p::setTimeout(const Timeout timeout) noexcept
{
    // Check the timeout value, return false if invalid.
    if (!isTimeoutValid(timeout)) { return false; }

    // Calculate the timeout value before the timed write sequence.
    const uint8_t mappedVal{mapTimeout(timeout)};

    // Update the watchdog timeout, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    WDTCSR |= (1UL << WDCE) | (1UL << WDE);
    WDTCSR = mappedVal;
    utils::globalInterruptEnable();

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();

    // Store the new timeout and return true to indicate success.
    myTimeout = timeout;
    return true;
}
} // namespace watchdog
} // namespace driver
