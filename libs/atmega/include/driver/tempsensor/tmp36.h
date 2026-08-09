/**
 * @brief TMP36 temperature sensor implementation.
 */
#pragma once

#include <stdint.h>

#include "driver/tempsensor/interface.h"

namespace driver
{
/** ADC (A/D converter) interface. */
namespace adc
{
class Interface;
}

namespace tempsensor
{
/**
 * @brief TMP36 temperature sensor implementation.
 *
 *        This class is non-copyable and non-movable.
 */
class Tmp36 final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] pin Pin the temperature sensor is connected to.
     * @param[in] adc A/D converter for reading the input voltage from the sensor.
     */
    explicit Tmp36(uint8_t pin, adc::Interface& adc) noexcept;

    /**
     * @brief Destructor.
     */
    ~Tmp36() noexcept override = default;

    /**
     * @brief Check if the temperature sensor is initialized.
     *
     * @return True if the temperature sensor is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Read the temperature sensor.
     *
     * @return The temperature in degrees Celsius.
     */
    int16_t read() const noexcept override;

    Tmp36()                        = delete; // No default constructor.
    Tmp36(const Tmp36&)            = delete; // No copy constructor.
    Tmp36(Tmp36&&)                 = delete; // No move constructor.
    Tmp36& operator=(const Tmp36&) = delete; // No copy assignment.
    Tmp36& operator=(Tmp36&&)      = delete; // No move assignment.

private:
    /** A/D converter to read the input voltage from the sensor. */
    adc::Interface& myAdc;

    /** Analog pin the temperature sensor is connected to. */
    const uint8_t myPin;
};
} // namespace tempsensor
} // namespace driver
