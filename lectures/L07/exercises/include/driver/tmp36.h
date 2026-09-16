/**
 * @file TMP36 stub driver.
 */
#pragma once

namespace driver
{
/**
 * @brief TMP36 stub driver structure.
 */
struct Tmp36 final
{
    /**
     * @brief Read temperature.
     *
     * @return Temperature in Celsius.
     */
    [[nodiscard]] double read() const noexcept
    {
        constexpr double offsetVoltage_v{0.5};     // Output voltage at 0 °C.
        constexpr double voltagePerDegree_v{0.01}; // Output voltage increase per °C.
        return (myInputVoltage_v - offsetVoltage_v) / voltagePerDegree_v;
    }

    /**
     * @brief Set input voltage.
     *
     * @param[in] voltage_v Input voltage in V. Must be in range [0.0, 5.0].
     */
    void setInputVoltage(const double voltage_v) noexcept
    {
        constexpr double min_v{0.0};
        constexpr double max_v{5.0};

        // Check the input voltage, do nothing if outside the supported range.
        if ((min_v > voltage_v) || (max_v < voltage_v)) { return; }
        myInputVoltage_v = voltage_v;
    }

private:
    /** Input voltage in V. */
    double myInputVoltage_v{};
};
} // namespace driver
