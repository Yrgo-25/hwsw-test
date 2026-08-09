/**
 * @brief ADC (A/D converter) interface.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace driver
{
namespace adc
{
/**
 * @brief ADC (A/D converter) interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the resolution of the ADC.
     *
     * @return The resolution of the ADC in bits.
     */
    virtual uint8_t resolution() const noexcept = 0;

    /**
     * @brief Get the maximal input value of the ADC.
     *
     * @return The maximum digital value of the ADC.
     */
    virtual uint16_t maxValue() const noexcept = 0;

    /**
     * @brief Get the supply voltage of the ADC.
     *
     * @return The supply voltage of the ADC in Volts.
     */
    virtual double supplyVoltage() const noexcept = 0;

    /**
     * @brief Read input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The digital value corresponding to the input of the specified channel.
     */
    virtual uint16_t read(uint8_t channel) const noexcept = 0;

    /**
     * @brief Calculate duty cycle out of input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The duty cycle as a floating point value between 0.0 - 1.0.
     */
    virtual double dutyCycle(uint8_t channel) const noexcept = 0;

    /**
     * @brief Read input voltage from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The input voltage in Volts.
     */
    virtual double inputVoltage(uint8_t channel) const noexcept = 0;

    /**
     * @brief Check whether the ADC is initialized.
     *
     * @return True if the ADC is initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Indicate whether the ADC is enabled.
     *
     * @return True if the ADC is enabled, false otherwise.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Set enablement of ADC.
     *
     * @param[in] enable Indicate whether to enable the ADC.
     */
    virtual void setEnabled(bool enable) noexcept = 0;

    /**
     * @brief Check whether the given channel is valid.
     *
     * @param[in] channel The channel to check.
     *
     * @return True if the channel is valid, false otherwise.
     */
    virtual bool isChannelValid(uint8_t channel) const noexcept = 0;
};
} // namespace adc
} // namespace driver
