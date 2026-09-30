/**
 * @brief Temperature sensor stub.
 */
#pragma once

#include <stdint.h>

#include "driver/tempsensor/interface.h"

namespace driver
{
namespace tempsensor
{
/**
 * @brief Temperature sensor stub.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Create a new temperature sensor stub.
     *
     * @param[in] initialTemp Initial temperature in degrees Celsius (default = 25).
     */
    explicit Stub(const int16_t initialTemp = 25) noexcept
        : myTemp{initialTemp}
        , myInitialized{true}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check if the temperature sensor is initialized.
     *
     * @return True if the temperature sensor is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return myInitialized; }

    /**
     * @brief Read the temperature sensor.
     *
     * @return The temperature in degrees Celsius.
     */
    int16_t read() const noexcept override { return myTemp; }

    /**
     * @brief Set the temperature (virtual input).
     *
     * @param[in] temp The new temperature in degrees Celsius.
     */
    void setTemp(const int16_t temp) noexcept { myTemp = temp; }

    /**
     * @brief Set initialization status of the temperature sensor.
     *
     * @param[in] initialized True to indicate that the temperature sensor is initialized.
     */
    void setInitialized(const bool initialized) noexcept { myInitialized = initialized; }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Temperature in degrees Celsius (virtual input). */
    int16_t myTemp;

    /** Indicate whether the temperature sensor is initialized. */
    bool myInitialized;
};
} // namespace tempsensor
} // namespace driver
