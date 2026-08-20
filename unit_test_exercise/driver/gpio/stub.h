/**
 * @file GPIO stub driver.
 */
#pragma once

#include "driver/gpio/interface.h"

namespace driver::gpio
{
/**
 * @brief GPIO stub driver.
 */
class Stub final : public Interface
{
public:
    /**
     *
     * @brief Constructor.
     * 
     * @param[in] initialState Initial GPIO state (default = false).
     */
    explicit Stub(const bool initialState = false) noexcept
        : myState{initialState}
    {}
    
    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Get GPIO state.
     * 
     * @return True if enabled, false if disabled.
     */
    [[nodiscard]] bool read() const noexcept override { return myState; }

    /**
     * @brief Set GPIO state.
     * 
     * @param[in] state New state (true = enabled, false = disabled).
     */
    void write(const bool state) noexcept override { myState = state; }

    /**
     * @brief Toggle GPIO state.
     */
    void toggle() noexcept override { myState = !myState; }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** GPIO state (true = enabled, false = disabled). */
    bool myState;
};
} // namespace driver::gpio
