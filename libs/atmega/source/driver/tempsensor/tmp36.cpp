/**
 * @brief TMP36 temperature sensor implementation details.
 */
#include <stdint.h>

#include "driver/adc/interface.h"
#include "driver/tempsensor/tmp36.h"
#include "utils/utils.h"

namespace driver
{
namespace tempsensor
{
// -----------------------------------------------------------------------------
Tmp36::Tmp36(const uint8_t pin, adc::Interface& adc) noexcept
    : myAdc{adc}
    , myPin{pin}
{
    // Enable the ADC if the initialization succeeded.
    if (isInitialized()) { myAdc.setEnabled(true); }
}

// -----------------------------------------------------------------------------
bool Tmp36::isInitialized() const noexcept
{
    // Return true if the temperature sensor pin is valid and the ADC is initialized.
    return myAdc.isChannelValid(myPin) && myAdc.isInitialized();
}

// -----------------------------------------------------------------------------
int16_t Tmp36::read() const noexcept
{
    // Return 0 if initialization failed.
    if (!isInitialized()) { return 0; }

    // Return the temperature, rounded to the nearest integer.
    const double temperature{100.0 * myAdc.inputVoltage(myPin) - 50.0};
    return utils::round<int16_t>(temperature);
}
} // namespace tempsensor
} // namespace driver
