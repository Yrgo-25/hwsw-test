/**
 * @brief Unit tests for the TMP36 temp sensor.
 */
#include <cstdint>
#include <memory>

#include "yrgo/test/test.h"

#include "driver/adc/stub.h"
#include "driver/tempsensor/tmp36.h"
#include "utils/utils.h"

#ifdef TESTSUITE

namespace driver
{
namespace
{
// -----------------------------------------------------------------------------
constexpr double computeInputVoltage(const std::uint16_t adcVal) noexcept
{
    constexpr double supplyVoltage{5.0};
    constexpr std::uint16_t adcMax{1023U};

    // Convert the ADC value to a voltage.
    return static_cast<double>(adcVal) / adcMax * supplyVoltage;
}

// -----------------------------------------------------------------------------
constexpr std::int16_t convertToTemp(const double inputVoltage) noexcept
{
    // Convert voltage to temperature: T(°C) = 100 * V - 50.
    return utils::round<std::int16_t>(100.0 * inputVoltage - 50.0);
}

// -----------------------------------------------------------------------------
constexpr std::int16_t convertToTemp(const std::uint16_t adcVal) noexcept
{
    // Convert ADC value to voltage, then to temperature.
    return convertToTemp(computeInputVoltage(adcVal));
}

/**
 * @brief Temp sensor initialization test.
 *
 *        Verify that the sensor isn't initialized if:
 *            - The ADC isn't initialized.
 *            - The temp sensor pin number (the ADC channel) is invalid.
 */
TEST(TempSensor_Tmp36, Initialization)
{
    constexpr std::uint16_t adcVal{100U};
    constexpr std::int16_t defaultTemp{0U};
    constexpr std::int16_t expectedTemp{convertToTemp(adcVal)};

    // Set up the ADC.
    adc::Stub adc{};
    adc.setValue(adcVal);

    // Case 1 - Simulate a valid pin.
    {
        // Create a temp sensor instance for this pin.
        constexpr std::uint8_t pin{0U};
        adc.setChannelValidity(true);
        adc.setInitialized(true);
        tempsensor::Tmp36 tempSensor{pin, adc};

        // For valid pins, the sensor should be initialized and return the expected temperature.
        EXPECT_TRUE(tempSensor.isInitialized());
        EXPECT_EQ(tempSensor.read(), expectedTemp);
    }

    // Case 2 - Simulate an invalid pin.
    {
        // Create a temp sensor instance for this pin.
        constexpr std::uint8_t pin{10U};
        adc.setChannelValidity(false);
        adc.setInitialized(true);
        tempsensor::Tmp36 tempSensor{pin, adc};

        // Expect the temp sensor to not be initialized and to return the default temperature.
        EXPECT_FALSE(tempSensor.isInitialized());
        EXPECT_EQ(tempSensor.read(), defaultTemp);
    }

    // Case 3 - Simulate that the ADC isn't initialized.
    {
        // Create a temp sensor instance.
        constexpr std::uint8_t pin{0U};
        adc.setChannelValidity(true);
        adc.setInitialized(false);
        tempsensor::Tmp36 tempSensor{pin, adc};

        // Expect the temp sensor to not be initialized and to return the default temperature.
        EXPECT_FALSE(tempSensor.isInitialized());
        EXPECT_EQ(tempSensor.read(), defaultTemp);
    }
}

/**
 * @brief Temp sensor accuracy test.
 *
 *        Verify that the temp sensor predicts accurately.
 */
TEST(TempSensor_Tmp36, Accuracy)
{
    constexpr std::uint8_t tempSensorPin{0U};
    constexpr std::uint16_t adcMax{1000U};
    constexpr std::size_t stepVal{10U};

    // Set up the ADC.
    adc::Stub adc{};

    // Set up the temp sensor, use the interface this time.
    std::unique_ptr<tempsensor::Interface> tempSensor{
        std::make_unique<tempsensor::Tmp36>(tempSensorPin, adc)};

    // Expect the temp sensor to be initialized successfully.
    EXPECT_TRUE(tempSensor->isInitialized());

    // Try different ADC values to simulate different input voltages.
    for (std::uint16_t adcVal{}; adcVal <= adcMax; adcVal += stepVal)
    {
        // Calculate the expected temperature for this ADC value.
        const std::int16_t expectedTemp{convertToTemp(adcVal)};

        // Set the ADC register to simulate the sensor reading.
        adc.setValue(adcVal);

        // The sensor should return the expected temperature for this ADC value.
        EXPECT_EQ(tempSensor->read(), expectedTemp);
    }
}
} // namespace
} // namespace driver

#endif /** TESTSUITE */
