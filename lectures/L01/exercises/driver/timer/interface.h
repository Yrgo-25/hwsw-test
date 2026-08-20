/**
 * @file Timer driver interface.
 */
#pragma once

#include <cstdint>

namespace driver::timer
{
/**
 * @brief Timer driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the configued timeout.
     *
     * @return Timeout in milliseconds.
     */
    [[nodiscard]] virtual std::uint16_t timeout_ms() const noexcept = 0;

    /**
     * @brief Check if the timer is running.
     *
     * @return True if running, false if stopped.
     */
    [[nodiscard]] virtual bool isRunning() const noexcept = 0;

    /**
     * @brief Start the timer.
     */
    virtual void start() noexcept = 0;

    /**
     * @brief Stop the timer.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Toggle the timer.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Advance the timer.
     */
    virtual void tick() noexcept = 0;

    /**
     * @brief Check if the timer has timed out.
     *
     * @return True on timeout, false if not.
     */
    [[nodiscard]] virtual bool hasTimedOut() const noexcept = 0;
};
} // namespace driver::timer
