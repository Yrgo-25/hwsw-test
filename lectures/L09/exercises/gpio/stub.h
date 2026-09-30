/**
 * @brief GPIO stub driver.
 */
#pragma once

#include <stdint.h>

#include "driver/gpio/interface.h"

namespace driver
{
namespace gpio
{
/**
 * @brief GPIO stub driver.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] mode Mode to configure (default = input).
     */
    explicit Stub(const Mode mode = Mode::Input) noexcept
        : myMode{mode}
        , myState{false}
        , myInitialized{true}
        , myInterruptEnabled{false}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check whether the GPIO is initialized.
     *
     *        An uninitialized device indicates that the specified PIN was unavailable or invalid
     *        when the device was created.
     *
     * @return True if the device is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return myInitialized; }

    /**
     * @brief Get the configured mode of the GPIO.
     *
     * @return The configured mode of the GPIO.
     */
    Mode mode() const noexcept override { return myMode; }

    /**
     * @brief Read input of the GPIO.
     *
     * @return True if the input is high, false otherwise.
     */
    bool read() const noexcept override { return myState; }

    /**
     * @brief Write output to the GPIO.
     *
     * @param[in] output The output value to write (true = high, false = low).
     */
    void write(const bool output) noexcept override
    {
        // Only update the state if the GPIO is working correctly.
        if (myInitialized) { myState = output; }
    }

    /**
     * @brief Toggle the output of the GPIO.
     */
    void toggle() noexcept override
    {
        // Only update the state if the GPIO is working correctly.
        if (myInitialized) { myState = !myState; }
    }

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the GPIO, false otherwise.
     */
    void enableInterrupt(const bool enable) noexcept override
    {
        // Update interrupt state if initialized.
        if (myInitialized) { myInterruptEnabled = enable; }
    }

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the I/O port, false otherwise.
     */
    void enableInterruptOnPort(const bool enable) noexcept override
    {
        // Update interrupt state if initialized.
        if (myInitialized) { myInterruptEnabled = enable; }
    }

    /**
     * @brief Check if interrupts are enabled.
     *
     * @return True if enabled, false if not.
     */
    bool isInterruptEnabled() const noexcept { return myInterruptEnabled; }

    /**
     * @brief Simulate initalization state.
     *
     * @param[in] initialized True if initialized, false if not.
     */
    void setInitialized(const bool initialized) noexcept
    {
        myInitialized = initialized;

        // Set state to low and disable interrupts if not initialized.
        if (!myInitialized)
        {
            myState            = false;
            myInterruptEnabled = false;
        }
    }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Configured GPIO mode. */
    const Mode myMode;

    /** GPIO state (true = high, false = low). */
    bool myState;

    /** True if initialized, false if not. */
    bool myInitialized;

    /** True if interrupts are enabled, false if not. */
    bool myInterruptEnabled;
};
} // namespace gpio
} // namespace driver
