/**
 * @brief Unit tests for the ATmega328p serial driver.
 */
#include <chrono>
#include <cstdint>
#include <exception>
#include <string>
#include <thread>

#include "yrgo/test/test.h"

#include "arch/avr/hw_platform.h"
#include "driver/serial/atmega328p.h"
#include "utils/utils.h"

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

/**
 * Upper bound for the simulated transmission. The threads below wait for each other via the
 * simulated UDRE0 flag, so an unfinished printThread() would otherwise leave them spinning
 * forever and hang the whole test suite instead of failing it.
 */
constexpr std::size_t TestTimeout_ms{2000U};

// -----------------------------------------------------------------------------
std::chrono::steady_clock::time_point deadline() noexcept
{
    return std::chrono::steady_clock::now() + std::chrono::milliseconds(TestTimeout_ms);
}

// -----------------------------------------------------------------------------
bool expired(const std::chrono::steady_clock::time_point& limit) noexcept
{
    return std::chrono::steady_clock::now() >= limit;
}

/**
 * @brief Run a thread body and capture an assertion failure instead of terminating.
 *
 *        EXPECT_* throws, and an exception that escapes a thread terminates the entire process
 *        without printing which assertion failed. The captured exception is rethrown on the main
 *        thread instead, so a failed assertion inside a thread is reported as a normal test
 *        failure.
 *
 * @tparam Callable Type of the thread body.
 *
 * @param[out] error Set to the caught exception, if any.
 * @param[in]  body  The thread body to run.
 */
template<typename Callable>
void runInThread(std::exception_ptr& error, Callable&& body) noexcept
{
    try
    {
        body();
    }
    catch (...)
    {
        error = std::current_exception();
    }
}

// -----------------------------------------------------------------------------
serial::Interface& initSerial() noexcept
{
    // Initialize and enable serial instance.
    serial::Interface& serial{serial::Atmega328p::getInstance()};
    serial.setEnabled(true);
    return serial;
}

// -----------------------------------------------------------------------------
void delay_us(const std::size_t duration_us) noexcept
{
    std::this_thread::sleep_for(std::chrono::microseconds(duration_us));
}

// -----------------------------------------------------------------------------
void simulateDataReg(const bool& stop) noexcept
{
    std::uint8_t prevByte{};

    // Give up after TestTimeout_ms, so an unfinished printThread() fails the test rather than
    // hanging the suite.
    const auto limit{deadline()};

    // Initialize UDR0 to 0 and set UDRE0 (data register empty, ready to write).
    UDR0 = 0U;
    utils::set(UCSR0A, UDRE0);

    // Clear UDRE0 when UDR0 changes to simulate that the data register is full.
    while (!stop && !expired(limit))
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
void printThread(serial::Interface& serial, const std::string& msg, bool& stop)
{
    //! @todo Implement this function!

    // Transmit the entire string.

    // Set the stop flag to true to signal that transmission is complete.
}

// -----------------------------------------------------------------------------
void readDataRegThread(const std::string& msg, const bool& stop)
{
    //! @todo Implement this function!

    // Give up after TestTimeout_ms, see simulateDataReg().
    const auto limit{deadline()};

    // Iterate through each character in the message.
    for (const auto& c : msg)
    {
        // Wait until data is available (UDRE0 is cleared by the hardware simulator).
        // Also check the stop flag to allow early termination.
        while (utils::read(UCSR0A, UDRE0) && !stop && !expired(limit))
        {
            delay_us(TransmissionDelay_us);
        }

        // If the stop flag is set, or the deadline passed, break out of the loop.
        if (stop || expired(limit)) { break; }

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
    //! - Check that baud rate can be read.
}

/**
 * @brief Serial print test.
 *
 *        Verify that messages are transmitted correctly.
 */
TEST(Serial_Atmega328p, Transmit)
{
    // Initialize and enable the serial driver.
    serial::Interface& serial{initSerial()};

    // Message to transmit.
    const std::string msg{"This is a message!\n"};

    // Stop flag shared between threads.
    bool stopFlag{false};

    // Assertion failures from the worker threads, rethrown on this thread after the join.
    std::exception_ptr printError{};
    std::exception_ptr readError{};

    // Create threads to simulate data transmission.
    std::thread t1{simulateDataReg, std::ref(stopFlag)};
    std::thread t2{[&] { runInThread(printError, [&] { printThread(serial, msg, stopFlag); }); }};
    std::thread t3{[&] { runInThread(readError, [&] { readDataRegThread(msg, stopFlag); }); }};

    // Synchronize the threads.
    t1.join();
    t2.join();
    t3.join();

    // Report any assertion that failed inside a worker thread.
    if (printError) { std::rethrow_exception(printError); }
    if (readError) { std::rethrow_exception(readError); }

    // printThread() sets the stop flag once the whole message has been transmitted. If it is
    // still false, the wait loops hit their timeout instead, i.e. the transmission never
    // completed.
    EXPECT_TRUE(stopFlag);
}

//! @todo Add more tests here!

} // namespace
} // namespace driver.

//! @todo Remove this #endif in lecture 3 to enable these tests.
#endif /** LECTURE3 */

#endif /** TESTSUITE */