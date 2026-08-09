/**
 * @brief Watchdog timer driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/watchdog/interface.h"

namespace driver
{
namespace watchdog
{
/**
 * @brief Watchdog timer driver for ATmega328P.
 *
 *        Use the singleton design pattern to ensure only one watchdog instance exists,
 *        reflecting the hardware limitation of a single watchdog on the MCU.
 *
 *        The default timeout is 1024 ms.
 */
class Atmega328p final : public Interface
{
public:
    /** Enumeration of watchdog timer timeouts. */
    enum class Timeout : uint16_t;

    /**
     * @brief Get the singleton watchdog timer instance.
     *
     * @return Reference to the singleton watchdog timer instance.
     */
    static Interface& getInstance() noexcept;

    /**
     * @brief Check whether the watchdog timer is initialized.
     *
     * @return True if the watchdog timer is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the watchdog timer is enabled.
     *
     * @return True if the watchdog timer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of the watchdog timer.
     *
     * @param[in] enable True to enable the watchdog timer, false otherwise.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Get the timeout of the watchdog timer.
     *
     * @return The timeout of the watchdog timer in milliseconds.
     */
    uint16_t timeout_ms() const noexcept override;

    /**
     * @brief Set timeout of the watchdog timer.
     *
     * @param[in] timeout Timeout of the watchdog timer in milliseconds.
     *
     * @return True if the timeout was set, false if the given timeout is invalid.
     */
    bool setTimeout_ms(uint16_t timeout_ms) noexcept override;

    /**
     * @brief Reset the watchdog timer.
     */
    void reset() noexcept override;

    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    Atmega328p() noexcept;
    ~Atmega328p() noexcept override = default;
    bool setTimeout(Timeout timeout) noexcept;

    /** Watchdog timeout. */
    Timeout myTimeout;

    /** Indicate whether the watchdog is enabled. */
    bool myEnabled;
};

/**
 * @brief Enumeration of watchdog timer timeouts.
 */
enum class Atmega328p::Timeout : uint16_t
{
    Duration16ms   = 16UL,   // 16 ms.
    Duration32ms   = 32UL,   // 32 ms.
    Duration64ms   = 64UL,   // 64 ms.
    Duration128ms  = 128UL,  // 128 ms.
    Duration256ms  = 256UL,  // 256 ms.
    Duration512ms  = 512UL,  // 512 ms.
    Duration1024ms = 1024UL, // 1024 ms.
    Duration2048ms = 2048UL, // 2048 ms.
    Duration4096ms = 4096UL, // 4096 ms.
    Duration8192ms = 8192UL, // 8192 ms.
    Invalid,                 // Invalid timeout.
};
} // namespace watchdog
} // namespace driver
