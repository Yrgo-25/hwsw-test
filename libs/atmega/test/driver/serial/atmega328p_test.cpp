/**
 * @brief Unit tests for the ATmega328p serial driver.
 */
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>

#include "arch/avr/hw_platform.h"
#include "driver/serial/atmega328p.h"
#include "utils/utils.h"
#include "yrgo/test/test.h"

#ifdef TESTSUITE

//! @todo Remove this #ifdef in lecture 3 to enable these tests.
#ifdef LECTURE3

//! @todo Implement tests according to project requirements.
namespace driver
{
namespace
{
/** Simulated transmission delay in microseconds. */
constexpr std::size_t TransmissionDelay_us{10U};

/** Expected baud rate in bps. */
constexpr std::uint16_t ExpectedBaudRate_bps{9600U};

// -----------------------------------------------------------------------------
[[nodiscard]] serial::Interface& initSerial() noexcept
{
    // Initialize and enable serial instance.
    auto& serial = serial::Atmega328p::getInstance();
    serial.setEnabled(true);
    return serial;
}

// -----------------------------------------------------------------------------
void delay_us(const std::size_t duration_us) noexcept
{
    std::this_thread::sleep_for(std::chrono::microseconds(duration_us));
}

// -----------------------------------------------------------------------------
void injectRxData(const char* buf) noexcept
{
    constexpr std::uint16_t timeout_us{1000U};

    // Inject each byte one by one.
    for (const char* c{buf}; '\0' != *c; ++c)
    {
        // Put the character in the data register, indicate that a character has been received.
        UDR0 = static_cast<std::uint8_t>(*c);
        utils::set(UCSR0A, RXC0);

        // Wait until RXC0 is cleared in the test, i.e. until the character has been read.
        // Stop waiting once the timeout has passed, so that the test cannot hang.
        for (std::size_t i{}; i < timeout_us; i += TransmissionDelay_us)
        {
            if (!utils::read(UCSR0A, RXC0)) { break; }
            delay_us(TransmissionDelay_us);
        }
    }
}

// -----------------------------------------------------------------------------
void simulateDataReg(const std::atomic<bool>& stop) noexcept
{
    std::uint8_t prevByte{};

    // Initialize UDR0 to 0 and set UDRE0 (data register empty, ready to write).
    UDR0 = 0U;
    utils::set(UCSR0A, UDRE0);

    // Clear UDRE0 when UDR0 changes to simulate that the data register is full.
    while (!stop.load())
    {
        constexpr std::size_t loopInterval_us{1U};

        // Read the current byte from UDR0.
        const std::uint8_t byte{UDR0};

        // Check if UDR0 has changed and is non-zero (new data written).
        if ((byte != prevByte) && (0U != byte))
        {
            prevByte = byte;

            // Simulate transmission delay, then mark that the data register is full.
            delay_us(TransmissionDelay_us);
            utils::clear(UCSR0A, UDRE0);
        }
        delay_us(loopInterval_us);
    }
}

// -----------------------------------------------------------------------------
void printThread(serial::Interface& serial, const char* msg, std::atomic<bool>& stop)
{
    //! @todo Implement this function!

    // Transmit the entire string.

    // Set the stop flag to true to signal that transmission is complete.
}

// -----------------------------------------------------------------------------
void readDataRegThread(const char* msg, const std::atomic<bool>& stop)
{
    //! @todo Implement this function!

    // Iterate through each character in the message.
    for (const char* c{msg}; '\0' != *c; ++c)
    {
        // Wait until data is available (UDRE0 is cleared by the hardware simulator).
        // Also check the stop flag to allow early termination.
        while (utils::read(UCSR0A, UDRE0) && !stop.load())
        {
            delay_us(TransmissionDelay_us);
        }

        // If the stop flag is set, break out of the loop.

        // Read the character from UDR0 and verify it matches the expected character.

        // Set UDRE0 to signal that the data has been read and the register is empty.

        //! @todo Remove this line once the character 'c' is checked.
        (void)(c);
    }
}

/**
 * @brief Serial initialization test.
 *
 *        Verify that the serial driver is initialized correctly.
 */
TEST(Serial_Atmega328p, Initialization)
{
    //! @todo Test serial initialization:
    //! - Verify that isInitialized() returns true.
    //! - Verify that the driver can be enabled/disabled.
    //! - Check that baud rate can be read correctly (with the value specified in the driver
    //!   documentation).
}

/**
 * @brief Serial print test.
 *
 *        Verify that messages are transmitted correctly.
 */
TEST(Serial_Atmega328p, Transmit)
{
    // Initialize and enable the serial driver.
    auto& serial = initSerial();

    // Message to transmit.
    const char* msg{"This is a message!\n"};

    // Stop flag shared between threads.
    std::atomic<bool> stop{false};

    // Create threads to simulate data transmission.
    std::thread t1{simulateDataReg, std::cref(stop)};
    std::thread t2{printThread, std::ref(serial), msg, std::ref(stop)};
    std::thread t3{readDataRegThread, std::ref(msg), std::cref(stop)};

    // Synchronize the threads.
    t1.join();
    t2.join();
    t3.join();
}

/**
 * @brief Serial read test.
 *
 *        Verify that received characters are read correctly.
 */
TEST(Serial_Atmega328p, Read)
{
    constexpr std::uint16_t timeout_ms{100U};
    constexpr std::uint16_t bufLen{32U};

    // Initialize and enable the serial driver.
    auto& serial = initSerial();

    // Read buffer, initialized to zero.
    std::uint8_t buf[bufLen]{};

    //! @todo Test the error handling of read():
    //! - Expect -1 to be returned when the read buffer is a nullptr.
    //! - Expect -1 to be returned when the buffer size is 0.
    //! - Expect 0 to be returned when no character is received before the timeout expires.
    //!   Tip: Clear RXC0 first, otherwise the driver reads whatever UDR0 holds at the moment.

    // Message to receive.
    const char* msg{"This is a message!\n"};

    // Create a thread injecting the message into the data register, one character at a time.
    std::thread t1{injectRxData, msg};

    // Number of characters read so far.
    std::uint16_t bytesRead{};

    // Read the message one character at a time, since the simulator provides one at a time.
    for (const char* c{msg}; ('\0' != *c) && (bufLen > bytesRead); ++c)
    {
        // Read a single character into buf[bytesRead] by invoking read().
        // Check the return value, break out of the loop if it isn't 1.

        // Increment bytesRead once the character has been read.

        // Clear RXC0 to simulate that the hardware clears the flag when UDR0 is read.
        // This also signals the simulator thread that the next character can be injected.
    }

    //! @todo Remove these lines once the serial driver and the read buffer are used.
    (void)(timeout_ms);
    (void)(serial);
    (void)(buf);
    (void)(bytesRead);

    // Synchronize the threads (this thread and the RX injector thread).
    t1.join();

    //! @todo Verify the read data. Do this after joining the thread, so that a failed check
    //! doesn't leave the simulator thread running:
    //! - Expect the number of read characters to match the length of the message.
    //! - Expect each character of the read buffer to match the corresponding character of the
    //!   message.
}
} // namespace
} // namespace driver

//! @todo Remove this #endif in lecture 3 to enable these tests.
#endif /** LECTURE3 */

#endif /** TESTSUITE */