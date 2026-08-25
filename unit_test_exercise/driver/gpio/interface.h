/**
 * @file GPIO driver interface.
 */
#pragma once

namespace driver::gpio
{
/**
 * @brief GPIO driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get GPIO state.
     *
     * @return True if enabled, false if disabled.
     */
    [[nodiscard]] virtual bool read() const noexcept = 0;

    /**
     * @brief Set GPIO state.
     *
     * @param[in] state New state (true = enabled, false = disabled).
     */
    virtual void write(bool state) noexcept = 0;

    /**
     * @brief Toggle GPIO state.
     */
    virtual void toggle() noexcept = 0;
};
} // namespace driver::gpio
