/**
 * @brief Timer stub driver.
 */
#pragma once

#include <stdint.h>

#include "driver/timer/interface.h"

namespace driver
{
namespace timer
{
/**
 * @brief Timer stub driver.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] timeout_ms Timeout in millisecond (default = 1000 ms).
     */
    explicit Stub(const uint32_t timeout_ms = 1000U) noexcept
        : myTimeout_ms{timeout_ms}
        , myRunning{false}
        , myInitialized{true}
        , myElapsed{false}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check if the timer is initialized.
     *
     *        An uninitialized timer indicates that no timer circuit was available when the timer
     *        was created.
     *
     * @return True if the timer is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return myInitialized; }

    /**
     * @brief Check whether the timer is enabled.
     *
     * @return True if the timer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override { return myRunning; }

    /**
     * @brief Check whether the timer has timed out.
     *
     * @return True if the timer has timed out, false otherwise.
     */
    bool hasTimedOut() const noexcept override { return myElapsed; }

    /**
     * @brief Get the timeout of the timer.
     *
     * @return The timeout in milliseconds.
     */
    uint32_t timeout_ms() const noexcept override { return myTimeout_ms; }

    /**
     * @brief Set timeout of the timer.
     *
     * @param[in] timeout_ms The new timeout in milliseconds.
     */
    void setTimeout_ms(const uint32_t timeout_ms) noexcept override
    {
        // Set new timeout if initialized.
        if (myInitialized) { myTimeout_ms = timeout_ms; }
    }

    /**
     * @brief Start the timer.
     */
    void start() noexcept override
    {
        // Do nothing if not initialized.
        if (!myInitialized) { return; }
        myRunning = true;
        myElapsed = false;
    }

    /**
     * @brief Stop the timer.
     */
    void stop() noexcept override
    {
        // Do nothing if not initialized.
        if (!myInitialized) { return; }
        myRunning = false;
        myElapsed = false;
    }

    /**
     * @brief Toggle the timer.
     */
    void toggle() noexcept override
    {
        // Do nothing if not initialized.
        if (!myInitialized) { return; }
        myRunning = !myRunning;
        myElapsed = false;
    }

    /**
     * @brief Restart the timer.
     */
    void restart() noexcept override
    {
        // Only start the timer.
        // Nothing else needs to be done, since the stub doesn't have a counter.
        start();
    }

    /**
     * @brief Simulate initialization status.
     *
     * @param[in] initialized True if initialized, false if not.
     */
    void setInitialized(const bool initialized) noexcept
    {
        myInitialized = initialized;

        // Stop the timer if not initialized.
        if (!myInitialized)
        {
            myRunning = false;
            myElapsed = false;
        }
    }

    /**
     * @brief Set timeout status.
     *
     * @param[in] timedOut True on timeout, false if not.
     */
    void setTimedOut(const bool timedOut) noexcept
    {
        // Set timeout status if initialized.
        if (myInitialized) { myElapsed = timedOut; }
    }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Timeout in ms. */
    uint32_t myTimeout_ms;

    /** True if running, false if stopped. */
    bool myRunning;

    /** True if initialized, false if not. */
    bool myInitialized;

    /** True if elapsed (= timed out), false if not. */
    bool myElapsed;
};
} // namespace timer
} // namespace driver
