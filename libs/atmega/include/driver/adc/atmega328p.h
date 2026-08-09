/**
 * @brief ADC driver for the ATmega328P ADC (A/D converter).
 */
#pragma once

#include <stdint.h>

#include "driver/adc/interface.h"

namespace driver
{
namespace adc
{
/**
 * @brief ADC driver for the ATmega328P ADC (A/D converter).
 *
 *        Use the singleton design pattern to ensure only one ADC instance exists,
 *        reflecting the hardware limitation of a single ADC on the MCU.
 */
class Atmega328p final : public Interface
{
public:
    struct Pin;  // Pin aliases for analog pins.
    struct Port; // Port aliases for analog pins.

    /**
     * @brief Get the singleton ADC instance.
     *
     * @return Reference to the singleton ADC instance.
     */
    static Interface& getInstance() noexcept;

    /**
     * @brief Get the resolution of the ADC.
     *
     * @return The resolution of the ADC in bits.
     */
    uint8_t resolution() const noexcept override;

    /**
     * @brief Get the maximal input value of the ADC.
     *
     * @return The maximum digital value of the ADC.
     */
    uint16_t maxValue() const noexcept override;

    /**
     * @brief Get the supply voltage of the ADC.
     *
     * @return The supply voltage of the ADC in Volts.
     */
    double supplyVoltage() const noexcept override;

    /**
     * @brief Read input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The digital value corresponding to the input of the specified channel.
     */
    uint16_t read(uint8_t channel) const noexcept override;

    /**
     * @brief Calculate duty cycle out of input from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The duty cycle as a floating point value between 0.0 - 1.0.
     */
    double dutyCycle(uint8_t channel) const noexcept override;

    /**
     * @brief Read input voltage from given channel.
     *
     * @param[in] channel Channel from which to read.
     *
     * @return The input voltage in Volts.
     */
    double inputVoltage(uint8_t channel) const noexcept override;

    /**
     * @brief Check whether the ADC is initialized.
     *
     * @return True if the ADC is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicate whether the ADC is enabled.
     *
     * @return True if the ADC is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of ADC.
     *
     * @param[in] enable Indicate whether to enable the ADC.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Check whether the given channel is valid.
     *
     * @param[in] channel The channel to check.
     *
     * @return True if the channel is valid, false otherwise.
     */
    bool isChannelValid(uint8_t channel) const noexcept override;

    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    Atmega328p() noexcept;
    ~Atmega328p() noexcept override = default;

    /** Indicate whether the ADC is enabled. */
    bool myEnabled;
};

/**
 * @brief Structure of pin names for analog pins.
 */
struct Atmega328p::Pin
{
    static constexpr uint8_t A0{0U}; // Pin A0 = 0.
    static constexpr uint8_t A1{1U}; // Pin A1 = 1.
    static constexpr uint8_t A2{2U}; // Pin A2 = 2.
    static constexpr uint8_t A3{3U}; // Pin A3 = 3.
    static constexpr uint8_t A4{4U}; // Pin A4 = 4.
    static constexpr uint8_t A5{5U}; // Pin A5 = 5.
};

/**
 * @brief Structure of port names for analog pins.
 */
struct Atmega328p::Port
{
    static constexpr uint8_t C0{14U}; // PORTC0 = pin 14.
    static constexpr uint8_t C1{15U}; // PORTC1 = pin 15.
    static constexpr uint8_t C2{16U}; // PORTC2 = pin 16.
    static constexpr uint8_t C3{17U}; // PORTC3 = pin 17.
    static constexpr uint8_t C4{18U}; // PORTC4 = pin 18.
    static constexpr uint8_t C5{19U}; // PORTC5 = pin 10.
};
} // namespace adc
} // namespace driver
