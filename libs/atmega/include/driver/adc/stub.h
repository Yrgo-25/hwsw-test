/**
 * @brief ADC driver stub.
 */
#pragma once

#include <math.h>
#include <stdint.h>

#include "driver/adc/interface.h"

namespace driver
{
namespace adc
{
/**
 * @brief ADC driver stub.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Create a new ADC stub.
     *
     * @param[in] resolution ADC resolution (default = 10 bits).
     * @param[in] supplyVoltage Supply voltage (default = 5 V).
     */
    explicit Stub(const uint8_t resolution = 10U, const double supplyVoltage = 5.0) noexcept
        : mySupplyVoltage{supplyVoltage}
        , myMaxVal{static_cast<uint16_t>(pow(2U, resolution) - 1U)}
        , myAdcVal{}
        , myResolution{resolution}
        , myInitialized{true}
        , myEnabled{true}
        , myChannelValid{true}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Get the resolution of the ADC.
     *
     * @return The resolution of the ADC in bits.
     */
    uint8_t resolution() const noexcept override { return myResolution; }

    /**
     * @brief Get the maximal input value of the ADC.
     *
     * @return The maximum digital value of the ADC.
     */
    uint16_t maxValue() const noexcept override { return myMaxVal; }

    /**
     * @brief Get the supply voltage of the ADC.
     *
     * @return The supply voltage of the ADC in Volts.
     */
    double supplyVoltage() const noexcept override { return mySupplyVoltage; }

    /**
     * @brief Read input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The digital value corresponding to the input of the specified channel.
     */
    uint16_t read(const uint8_t channel) const noexcept override
    {
        (void)(channel);
        return myEnabled ? myAdcVal : 0U;
    }

    /**
     * @brief Calculate duty cycle out of input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The duty cycle as a floating point value between 0.0 - 1.0.
     */
    double dutyCycle(const uint8_t channel) const noexcept override
    {
        // Enforce floating-point division.
        return read(channel) / static_cast<double>(myMaxVal);
    }

    /**
     * @brief Read input voltage from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The input voltage in Volts.
     */
    double inputVoltage(const uint8_t channel) const noexcept override
    {
        return dutyCycle(channel) * mySupplyVoltage;
    }

    /**
     * @brief Check whether the ADC is initialized.
     *
     * @return True if the ADC is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return myInitialized; }

    /**
     * @brief Indicate whether the ADC is enabled.
     *
     * @return True if the ADC is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override { return myEnabled; }

    /**
     * @brief Set enablement status of the ADC.
     *
     * @param[in] enable True to enable the ADC.
     */
    void setEnabled(const bool enable) noexcept override { myEnabled = enable; }

    /**
     * @brief Check whether the given channel is valid.
     *
     * @param[in] channel The channel to check.
     *
     * @return True if the channel is valid, false otherwise.
     */
    bool isChannelValid(const uint8_t channel) const noexcept override
    {
        (void)(channel);
        return myChannelValid;
    }

    /**
     * @brief Set channel validity for all channels.
     *
     * @param[in] valid True to validate all channels, false otherwise.
     */
    void setChannelValidity(const bool valid) noexcept { myChannelValid = valid; }

    /**
     * @brief Set the ADC value (virtual input).
     *
     * @param[in] value ADC value.
     */
    void setValue(const uint16_t value) noexcept
    {
        // Set the ADC value if valid.
        if (myMaxVal >= value) { myAdcVal = value; }
    }

    /**
     * @brief Set initialization status of the ADC.
     *
     * @param[in] initialized True to indicate that the ADC is initialized.
     */
    void setInitialized(const bool initialized) noexcept { myInitialized = initialized; }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Supply voltage. */
    const double mySupplyVoltage;

    /** ADC max value. */
    const uint16_t myMaxVal;

    /** ADC value (virtual input). */
    uint16_t myAdcVal;

    /** ADC resolution. */
    const uint8_t myResolution;

    /** Indicate whether the ADC is initialized. */
    bool myInitialized;

    /** Indicate whether the ADC is enabled. */
    bool myEnabled;

    /** Channel validity (all channels). */
    bool myChannelValid;
};
} // namespace adc
} // namespace driver
