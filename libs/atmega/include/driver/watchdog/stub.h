/**
 * @brief Watchdog timer stub.
 */
#pragma once

#include <stdint.h>

#include "driver/watchdog/interface.h"

namespace driver
{
namespace watchdog
{
/**
 * @brief Watchdog timer stub.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] timeout_ms Watchdog timeout in ms (default = 1024 ms).
     */
    Stub(const uint16_t timeout_ms = 1024U) noexcept
        : myTimeout_ms{timeout_ms}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check whether the watchdog timer is initialized.
     *
     * @return True if the watchdog timer is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return true; }

    /**
     * @brief Check whether the watchdog timer is enabled.
     *
     * @return True if the watchdog timer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override { return myEnabled; }

    /**
     * @brief Set enablement of the watchdog timer.
     *
     * @param[in] enable True to enable the watchdog timer, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override { myEnabled = enable; }

    /**
     * @brief Get timeout of the watchdog timer.
     *
     * @return Timeout of the watchdog timer in milliseconds.
     */
    uint16_t timeout_ms() const noexcept override { return myTimeout_ms; }

    /**
     * @brief Set timeout of the watchdog timer.
     *
     * @param[in] timeout_ms Timeout of the watchdog timer in milliseconds.
     *
     * @return True if the timeout was set, false if the given timeout is invalid.
     */
    bool setTimeout_ms(const uint16_t timeout_ms) noexcept override
    {
        // Stubs accept any value - no hardware constraints to enforce.
        myTimeout_ms = timeout_ms;
        return true;
    }

    /**
     * @brief Reset the watchdog timer.
     */
    void reset() noexcept override {}

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Watchdog timeout in ms. */
    uint16_t myTimeout_ms;

    /** Indicate whether the watchdog is enabled. */
    bool myEnabled;
};
} // namespace watchdog
} // namespace driver
