/**
 * @brief GPIO interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
namespace gpio
{
/**
 * @brief Enumeration of GPIO modes.
 */
enum class Mode : uint8_t
{
    Input,       // Input without internal pull-up resistor enabled (tri-state).
    InputPullup, // Input with internal pull-up resistor enabled.
    Output,      // Output.
    Count,       // Number of supported modes.
};

/**
 * @brief GPIO interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check whether the GPIO is initialized.
     *
     *        An uninitialized device indicates that the specified PIN was unavailable or invalid
     *        when the device was created.
     *
     * @return True if the device is initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Get the configured mode of the GPIO.
     *
     * @return The configured mode of the GPIO.
     */
    virtual Mode mode() const noexcept = 0;

    /**
     * @brief Read input of the GPIO.
     *
     * @return True if the input is high, false otherwise.
     */
    virtual bool read() const noexcept = 0;

    /**
     * @brief Write output to the GPIO.
     *
     * @param[in] output The output value to write (true = high, false = low).
     */
    virtual void write(bool output) noexcept = 0;

    /**
     * @brief Toggle the output of the GPIO.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the GPIO, false otherwise.
     */
    virtual void enableInterrupt(bool enable) noexcept = 0;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the I/O port, false otherwise.
     */
    virtual void enableInterruptOnPort(bool enable) noexcept = 0;
};
} // namespace gpio
} // namespace driver
