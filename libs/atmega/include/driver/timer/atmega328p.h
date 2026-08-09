/**
 * @brief Timer driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/timer/interface.h"

namespace driver
{
namespace timer
{
/**
 * @brief Timer driver for ATmega328P.
 *
 *        This class is non-copyable and non-movable.
 *
 * @note Tree hardware timers Timer 0 - Timer 2 are available.
 */
class Atmega328p final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] timeout_ms The timeout in milliseconds. Must be greater than 0.
     * @param[in] callback Callback to invoke on timeout (default = none).
     * @param[in] startTimer Start the timer immediately (default = false).
     */
    explicit Atmega328p(uint32_t timeout_ms, void (*callback)() = nullptr,
                        bool startTimer = false) noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept override;

    /**
     * @brief Check if the timer is initialized.
     *
     *        An uninitialized timer indicates that no timer circuit was available when the
     *        timer was created, or that the given timeout was invalid.
     *
     * @return True if the timer is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the timer is enabled.
     *
     * @return True if the timer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Check whether the timer has timed out.
     *
     * @return True if the timer has timed out, false otherwise.
     */
    bool hasTimedOut() const noexcept override;

    /**
     * @brief Get the timeout of the timer.
     *
     * @return The timeout in milliseconds.
     */
    uint32_t timeout_ms() const noexcept override;

    /**
     * @brief Set timeout of the timer.
     *
     * @param[in] timeout_ms The new timeout in milliseconds. Must be greater than 0.
     */
    void setTimeout_ms(uint32_t timeout_ms) noexcept override;

    /**
     * @brief Start the timer.
     */
    void start() noexcept override;

    /**
     * @brief Stop the timer.
     */
    void stop() noexcept override;

    /**
     * @brief Toggle the timer.
     */
    void toggle() noexcept override;

    /**
     * @brief Restart the timer.
     */
    void restart() noexcept override;

    /**
     * @brief Callback handler.
     */
    void handleCallback() noexcept;

    Atmega328p()                             = delete; // No default constructor.
    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    void addCallback(void (*callback)()) const noexcept;
    void removeCallback() const noexcept;
    bool increment() noexcept;
    void clearTimedOut() noexcept;

    /** Timer hardware structure. */
    struct Hardware;

    /** Hardware structure associated with the timer. */
    Hardware* myHw;

    /** Max value to count up to. */
    uint32_t myMaxCount;

    /** Indicate whether the timer is enabled. */
    bool myEnabled;
};
} // namespace timer
} // namespace driver
