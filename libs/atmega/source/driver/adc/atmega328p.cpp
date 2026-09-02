/**
 * @brief ADC driver implementation details for the ATmega328P ADC (A/D converter).
 */
#include <stdint.h>

#include "arch/avr/hw_platform.h"
#include "driver/adc/atmega328p.h"
#include "utils/utils.h"

namespace driver
{
namespace adc
{
namespace
{
/**
 * @brief Structure of ATmega328P ADC parameters.
 */
struct AdcParam
{
    /** Resolution in bits. */
    static constexpr uint8_t Resolution{10U};

    /** Max value of the ADC (limited by the resolution). */
    static constexpr uint16_t MaxValue{1023U};

    /** Supply voltage in Volts. */
    static constexpr double SupplyVoltage{5.0};

    /** ADC port offset (pin [14:19] == port [A0:A5]). */
    static constexpr uint8_t PortOffset{14U};
};

// -----------------------------------------------------------------------------
constexpr uint8_t normalizeChannel(const uint8_t channel) noexcept
{
    return Atmega328p::Pin::A5 >= channel ? channel : channel - AdcParam::PortOffset;
}

// -----------------------------------------------------------------------------
uint16_t adcValue(const uint8_t channel) noexcept
{
    ADMUX = (1U << REFS0) | normalizeChannel(channel);
    utils::set(ADCSRA, ADEN, ADSC, ADPS0, ADPS1, ADPS2);
    while (!utils::read(ADCSRA, ADIF))
        ;
    utils::set(ADCSRA, ADIF);
    return ADC;
}
} // namespace

// -----------------------------------------------------------------------------
Interface& Atmega328p::getInstance() noexcept
{
    // Create and initialize the singleton ADC instance (once only).
    static Atmega328p myInstance{};

    // Return a reference to the singleton ADC instance, cast to the corresponding interface.
    return myInstance;
}

// -----------------------------------------------------------------------------
uint8_t Atmega328p::resolution() const noexcept { return AdcParam::Resolution; }

// -----------------------------------------------------------------------------
uint16_t Atmega328p::maxValue() const noexcept { return AdcParam::MaxValue; }

// -----------------------------------------------------------------------------
double Atmega328p::supplyVoltage() const noexcept { return AdcParam::SupplyVoltage; }

// -----------------------------------------------------------------------------
uint16_t Atmega328p::read(const uint8_t channel) const noexcept
{
    return myEnabled && isChannelValid(channel) ? adcValue(channel) : 0U;
}

// -----------------------------------------------------------------------------
double Atmega328p::dutyCycle(const uint8_t channel) const noexcept
{
    return read(channel) / static_cast<double>(AdcParam::MaxValue);
}

// -----------------------------------------------------------------------------
double Atmega328p::inputVoltage(const uint8_t channel) const noexcept
{
    return dutyCycle(channel) * AdcParam::SupplyVoltage;
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Atmega328p::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Atmega328p::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
bool Atmega328p::isChannelValid(const uint8_t channel) const noexcept
{
    return utils::inRange(channel, Pin::A0, Pin::A5) || utils::inRange(channel, Port::C0, Port::C5);
}

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p() noexcept
    : myEnabled{true}
{
    read(Pin::A0);
}
} // namespace adc
} // namespace driver
