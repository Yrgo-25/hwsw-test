/**
 * @brief Logic stub for exposing protected members and constants for unit tests.
 */
#pragma once

#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "logic/logic.h"

namespace logic
{
/**
 * @brief Logic stub for testing.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Logic
{
public:
    /** Inherit constructors from Logic. */
    using Logic::Logic;

    /** Expose protected methods from Logic as public for testing purposes. */
    using Logic::readToggleStateFromEeprom;
    using Logic::toggleStateAddr;
    using Logic::writeToggleStateToEeprom;

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Print the temperature in the terminal.
     */
    void printTemperature() noexcept override
    {
        // Read and print the temperature.
        serial().printf("Simulated temperature: %d Celsius\n", tempSensor().read());
        myTempPrintouts++;
    }

    /**
     * @brief Get the number of temperature printouts.
     *
     * @return The number of temperature printouts.
     */
    uint16_t tempPrintoutCount() const noexcept { return myTempPrintouts; }

    Stub()                       = delete; // No default constructor.
    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** The number of temperature printouts. */
    uint16_t myTempPrintouts{};
};
} // namespace logic