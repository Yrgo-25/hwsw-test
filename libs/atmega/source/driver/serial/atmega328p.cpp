/**
 * @brief Implementation details of serial driver.
 */
#include "driver/serial/atmega328p.h"
#include "arch/avr/hw_platform.h"
#include "utils/utils.h"

namespace driver
{
namespace serial
{
namespace
{
/** Baud rate in bps. */
constexpr uint32_t BaudRate_bps{9600U};

/** New line character. */
constexpr char NewLine{'\n'};

/** Carriage return character. */
constexpr char CarriageReturn{'\r'};

// -----------------------------------------------------------------------------
void transmitChar(const char character) noexcept
{
    // Wait until the previous character has been sent.
    while (!utils::read(UCSR0A, UDRE0))
        ;

    // Put the new character in the transmission register.
    UDR0 = character;
}
} // namespace

// -----------------------------------------------------------------------------
Interface& Atmega328p::getInstance() noexcept
{
    // Create and initialize the singleton serial instance (once only).
    static Atmega328p myInstance{};

    // Return a reference to the singleton serial instance, cast to the corresponding interface.
    return myInstance;
}

// -----------------------------------------------------------------------------
uint32_t Atmega328p::baudRate_bps() const noexcept { return BaudRate_bps; }

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Atmega328p::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Atmega328p::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
int16_t Atmega328p::read(uint8_t* buffer, const uint16_t size,
                         const uint16_t timeout_ms) const noexcept
{
    // Check the input parameters, return -1 if invalid.
    if ((nullptr == buffer) || (size == 0U)) { return -1; }

    uint16_t bytesRead{};

    if (0U == timeout_ms)
    {
        // Read indefinitely until the buffer is full if no timeout has been specified.
        while (bytesRead < size)
        {
            // Wait until a byte has been received.
            while (!utils::read(UCSR0A, RXC0))
                ;
            buffer[bytesRead++] = UDR0;
        }
    }
    else
    {
        // Read until timeout has occurred or until the buffer is full.
        for (uint16_t i{}; i < timeout_ms; ++i)
        {
            // Read all available bytes.
            while ((size > bytesRead) && utils::read(UCSR0A, RXC0))
            {
                buffer[bytesRead++] = UDR0;
            }

            // Stop reading if the read buffer is full.
            if (size == bytesRead) { break; }

            // Wait a millisecond before reading again.
            utils::delay_ms(1U);
        }
    }
    // Return the number of bytes read.
    return static_cast<int16_t>(bytesRead);
}

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p() noexcept
    : myEnabled{true}
{
    // Baud rate value corresponding to 9600 kbps.
    constexpr uint16_t baudRateValue{103U};

    // Enable UART transmission.
    utils::set(UCSR0B, TXEN0, RXEN0);

    // Set the data size to eight bits per byte.
    utils::set(UCSR0C, UCSZ00, UCSZ01);

    // Set the baud rate to 9600 kbps.
    UBRR0 = baudRateValue;

    // Send carriage return to align the first message left.
    UDR0 = CarriageReturn;
}

// -----------------------------------------------------------------------------
void Atmega328p::print(const char* message) const noexcept
{
    // Terminate the function if serial transmission isn't enabled.
    if (!myEnabled) { return; }

    // Transmit each character of the string one by one.
    for (const char* it{message}; *it; ++it)
    {
        // Always combine new lines with carriage returns.
        if ((NewLine == *it) || (CarriageReturn == *it))
        {
            transmitChar(NewLine);
            transmitChar(CarriageReturn);
        }
        else { transmitChar(*it); }
    }
}
} // namespace serial
} // namespace driver
