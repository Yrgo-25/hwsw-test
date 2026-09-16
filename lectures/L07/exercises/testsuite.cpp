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
    //! @todo Add the test case as specified in Appendix B!
}

/**
 * @brief Run all tests.
 *
 * @return 0 on success, -1 on failure.
 */
int main() { return yrgo::test::runAllTests() ? 0 : -1; }
