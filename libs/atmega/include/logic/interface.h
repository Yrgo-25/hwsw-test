/**
 * @brief Generic logic interface for an MCU with configurable hardware devices.
 */
#pragma once

namespace logic
{
/**
 * @brief Generic logic for an MCU with configurable hardware devices.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check whether the logic implementation was initialized correctly.
     *
     * @return True if the implementation was initialized correctly, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Run the system.
     *
     * @param[in] stop Reference to stop flag.
     */
    virtual void run(const bool& stop) noexcept = 0;

    /**
     * @brief Handle button event.
     *
     *        Pin change interrupts are disabled for a debounce period after detecting button
     *        activity to mitigate the effects of contact bounce.
     */
    virtual void handleButtonEvent() noexcept = 0;

    /**
     * @brief Handle debounce timer timerout.
     *
     *        Enable pin change interrupts after a debounce period following button activity to
     *        mitigate the effects of contact bounce.
     */
    virtual void handleDebounceTimerTimeout() noexcept = 0;

    /**
     * @brief Handle toggle timer timeout.
     *
     *        Toggle the LED when the associated timer is enabled.
     */
    virtual void handleToggleTimerTimeout() noexcept = 0;

    /**
     * @brief Handle temperature timer timeout.
     *
     *        Read the surrounding temperature.
     */
    virtual void handleTempTimerTimeout() noexcept = 0;
};
} // namespace logic