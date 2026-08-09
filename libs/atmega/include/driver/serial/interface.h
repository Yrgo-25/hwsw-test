/**
 * @brief Serial driver interface.
 */
#pragma once

#include <stdint.h>
#include <stdio.h>

namespace driver
{
namespace serial
{
/**
 * @brief Serial driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the baud rate of the serial device.
     *
     * @return The baud rate in bps (bits per second).
     */
    virtual uint32_t baudRate_bps() const noexcept = 0;

    /**
     * @brief Check whether the serial device is initialized.
     *
     * @return True if the device is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check whether the serial device is enabled.
     *
     * @return True if the serial device is enabled, otherwise false.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Set enablement of serial device.
     *
     * @param[in] enable Indicate whether to enable the device.
     */
    virtual void setEnabled(bool enable) noexcept = 0;

    /**
     * @brief Read data from the serial port.
     *
     * @param[out] buffer Read buffer.
     * @param[in] size Buffer size in bytes.
     * @param[in] timeout_ms Read timeout. Pass 0 to wait indefinitely.
     *
     * @return The number of read characters, or -1 on error.
     */
    virtual int16_t read(uint8_t* buffer, uint16_t size, uint16_t timeout_ms) const noexcept = 0;

    /**
     * @brief Print formatted string to the serial port.
     *
     *        If the formatted string contains format specifiers, the additional arguments are
     *        formatted and inserted into the format string.
     *
     * @tparam Args  Parameter pack containing an arbitrary number of arguments.
     *
     * @param[in] format Reference to string to print.
     * @param[in] args Parameter pack containing potential additional arguments.
     *
     * @return True if the string was printed, false otherwise.
     */
    template<typename... Args>
    bool printf(const char* format, const Args&... args) const noexcept;

private:
    /**
     * @brief Print the given string in the serial terminal.
     *
     * @param[in] str The string to print.
     */
    virtual void print(const char* str) const noexcept = 0;
};

// -----------------------------------------------------------------------------
template<typename... Args>
bool Interface::printf(const char* format, const Args&... args) const noexcept
{
    // Return false if the format string is invalid.
    if (nullptr == format) { return false; }

    // Format and insert given additional arguments (if any).
    if (0U < sizeof...(args))
    {
        constexpr size_t length{101U};
        char buffer[length]{'\0'};

// Suppress -Wformat-security: 'format' is always a developer-controlled string,
// never user-supplied, so this use of snprintf is safe.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        (void)(snprintf(buffer, length, format, args...));
#pragma GCC diagnostic pop
        print(buffer);
    }
    // Print the string, then return true to indicate success.
    else { print(format); }
    return true;
}
} // namespace serial
} // namespace driver
