/**
 * @file Timer stub driver.
 */
#pragma once

#include <cstdint>
#include <cstdio>
#include <exception>

#include "driver/timer/interface.h"

namespace driver::timer
{
/**
 * @brief Timer stub driver.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] timeout_ms Timeout in milliseconds. Must be greater than 0.
     * @param[in] start True to immediately start the timer (default = 0).
     */
    explicit Stub(const std::uint16_t timeout_ms, const bool start = false) noexcept
        : myTimeout_ms{timeout_ms}
        , myCounter_ms{}
        , myRunning{start}
    {
        // Check if the timeout is valid, terminate the program if not.
        if (0U == timeout_ms)
        {
            std::fprintf(stderr, "Cannot create timer without a timeout!\n");
            std::terminate();
        }
    }

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Get the configued timeout.
     *
     * @return Timeout in milliseconds.
     */
    [[nodiscard]] std::uint16_t timeout_ms() const noexcept override { return myTimeout_ms; }

    /**
     * @brief Check if the timer is running.
     *
     * @return True if running, false if stopped.
     */
    [[nodiscard]] bool isRunning() const noexcept override { return myRunning; }

    /**
     * @brief Start the timer.
     */
    void start() noexcept override
    {
        myRunning    = true;
        myCounter_ms = 0U;
    }

    /**
     * @brief Stop the timer.
     */
    void stop() noexcept override { myRunning = false; }

    /**
     * @brief Toggle the timer.
     */
    void toggle() noexcept override
    {
        myRunning    = !myRunning;
        myCounter_ms = 0U;
    }

    /**
     * @brief Advance the timer.
     */
    void tick() noexcept override
    {
        if (myRunning && (myTimeout_ms > myCounter_ms)) { myCounter_ms++; }
    }

    /**
     * @brief Check if the timer has timed out.
     *
     * @return True on timeout, false if not.
     */
    [[nodiscard]] bool hasTimedOut() const noexcept override
    {
        const auto timeout = myTimeout_ms <= myCounter_ms;
        if (timeout) { myCounter_ms = 0U; }
        return timeout;
    }

    Stub()                       = delete; // No default constructor.
    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Configured timeout in ms. */
    const std::uint16_t myTimeout_ms;

    /** Internal millisecond counter. */
    mutable std::uint16_t myCounter_ms;

    /** True if running, false if stopped. */
    bool myRunning;
};
} // namespace driver::timer
