/**
 * @brief Watchdog timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
namespace watchdog
{
/**
 * @brief Watchdog timer interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check whether the watchdog timer is initialized.
     *
     * @return True if the watchdog timer is initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check whether the watchdog timer is enabled.
     *
     * @return True if the watchdog timer is enabled, false otherwise.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Set enablement of the watchdog timer.
     *
     * @param[in] enable True to enable the watchdog timer, false otherwise.
     */
    virtual void setEnabled(bool enable) noexcept = 0;

    /**
     * @brief Get timeout of the watchdog timer.
     *
     * @return Timeout of the watchdog timer in milliseconds.
     */
    virtual uint16_t timeout_ms() const noexcept = 0;

    /**
     * @brief Set timeout of the watchdog timer.
     *
     * @param[in] timeout Timeout of the watchdog timer in milliseconds.
     *
     * @return True if the timeout was set, false if the given timeout is invalid.
     */
    virtual bool setTimeout_ms(uint16_t timeout_ms) noexcept = 0;

    /**
     * @brief Reset the watchdog timer.
     */
    virtual void reset() noexcept = 0;
};
} // namespace watchdog
} // namespace driver
