/**
 * @brief Timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
namespace timer
{
/**
 * @brief Timer interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the timer is initialized.
     *
     *        An uninitialized timer indicates that no timer circuit was available when the timer
     *        was created.
     *
     * @return True if the timer is initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check whether the timer is enabled.
     *
     * @return True if the timer is enabled, false otherwise.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Check whether the timer has timed out.
     *
     * @return True if the timer has timed out, false otherwise.
     */
    virtual bool hasTimedOut() const noexcept = 0;

    /**
     * @brief Get the timeout of the timer.
     *
     * @return The timeout in milliseconds.
     */
    virtual uint32_t timeout_ms() const noexcept = 0;

    /**
     * @brief Set timeout of the timer.
     *
     * @param[in] timeout_ms The new timeout in milliseconds.
     */
    virtual void setTimeout_ms(uint32_t timeout_ms) noexcept = 0;

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
     * @brief Restart the timer.
     */
    virtual void restart() noexcept = 0;
};
} // namespace timer
} // namespace driver
