/**
 * @brief Temperature sensor interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
namespace tempsensor
{
/**
 * @brief Temperature sensor interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the temperature sensor is initialized.
     *
     * @return True if the temperature sensor is initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Read the temperature sensor.
     *
     * @return The temperature in degrees Celsius.
     */
    virtual int16_t read() const noexcept = 0;
};
} // namespace tempsensor
} // namespace driver
