/**
 * @file Unit test demo with floating-point numbers.
 */
#include <cstdint>

#include "driver/tmp36.h"
#include "yrgo/test/test.h"

/**
 * @brief Compute the input voltage corresponding to a given ADC value.
 *
 * @param[in] adcVal The ADC value to convert.
 *
 * @return The corresponding input voltage in V.
 */
[[nodiscard]] constexpr double computeInputVoltage(const std::uint16_t adcVal) noexcept
{
    constexpr double supplyVoltage{5.0};
    constexpr std::uint16_t adcMax{1023U};
    return static_cast<double>(adcVal) / adcMax * supplyVoltage;
}

/**
 * @brief Convert an ADC value to a temperature.
 *
 * @param[in] adcVal The ADC value to convert.
 *
 * @return The corresponding temperature in degrees Celsius.
 */
[[nodiscard]] constexpr double convertToTemp(const std::uint16_t adcVal) noexcept
{
    const auto voltage = computeInputVoltage(adcVal);
    return 100.0 * voltage - 50.0;
}

/**
 * @brief Test the temperature conversion over a range.
 */
TEST(Temperature, Accuracy)
{
    constexpr double tol{1e-9};
    constexpr std::uint16_t adcMax{1023U};
    driver::Tmp36 tmp36{};

    // Test each and every ADC value in the range [0, 1023].
    for (std::uint16_t adcVal{}; adcVal <= adcMax; ++adcVal)
    {
        // Set input voltage (since we don't have an ADC).
        const auto inputVoltage = computeInputVoltage(adcVal);
        tmp36.setInputVoltage(inputVoltage);

        // Compare expected and actual temperature.
        // Expect the values to be very close to each other.
        const auto expectedTemp = convertToTemp(adcVal);
        const auto actualTemp   = tmp36.read();
        EXPECT_NEAR(expectedTemp, actualTemp, tol);
    }
}

/**
 * @brief Run all tests.
 *
 * @return 0 on success, -1 on failure.
 */
int main() { return yrgo::test::runAllTests() ? 0 : -1; }
