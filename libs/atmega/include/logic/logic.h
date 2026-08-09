/**
 * @brief Generic logic implementation for an MCU with configurable hardware devices.
 */
#pragma once

#include <cstdint>

#include "logic/interface.h"

namespace driver
{
/** ADC (A/D converter) interface. */
namespace adc
{
class Interface;
}

/** EEPROM (Electrically Erasable Programmable ROM) stream interface. */
namespace eeprom
{
class Interface;
}

/** GPIO interface. */
namespace gpio
{
class Interface;
}

/** Serial transmission interface. */
namespace serial
{
class Interface;
}

/** Temperature sensor interface. */
namespace tempsensor
{
class Interface;
}

/** Timer interface. */
namespace timer
{
class Interface;
}

/** Watchdog timer interface. */
namespace watchdog
{
class Interface;
}

} // namespace driver

namespace logic
{
/**
 * @brief Generic logic for an MCU with configurable hardware devices.
 *
 *        The following devices are used:
 *            - A button to toggle a blink timer.
 *            - A button to read the surrounding temperature.
 *            - A blink timer to toggle an LED when enabled.
 *            - A temperature timer to print the temperature on timeout.
 *            - A debounce timer to reduce the effect of contact bounces after pushing the buttons.
 *            - A serial device to print serial data via UART.
 *            - A watchdog timer to restart the program if it gets stuck somewhere.
 *            - An EEPROM stream to store the LED state. On startup, this value is read; if the
 *              last stored state before power down was "on," the LED will automatically blink.
 *            - A temperature sensor to read the surrounding temperature.
 *
 *        This class is non-copyable and non-movable.
 */
class Logic : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] led The LED to toggle.
     * @param[in] toggleButton Button to toggle the toggle timer.
     * @param[in] tempButton Button to read the temperature.
     * @param[in] debounceTimer Timer to mitigate effects of contact bounces.
     * @param[in] toggleTimer Timer to toggle the LED.
     * @param[in] tempTimer Timer to read the temperature.
     * @param[in] serial Serial device to print status messages.
     * @param[in] watchdog Watchdog timer that resets the program if it becomes unresponsive.
     * @param[in] eeprom EEPROM stream to write the status of the LED to EEPROM.
     * @param[in] tempSensor Temperature sensor.
     */
    explicit Logic(driver::gpio::Interface& led, driver::gpio::Interface& toggleButton,
                   driver::gpio::Interface& tempButton, driver::timer::Interface& debounceTimer,
                   driver::timer::Interface& toggleTimer, driver::timer::Interface& tempTimer,
                   driver::serial::Interface& serial, driver::watchdog::Interface& watchdog,
                   driver::eeprom::Interface& eeprom,
                   driver::tempsensor::Interface& tempSensor) noexcept;

    /**
     * @brief Destructor.
     */
    ~Logic() noexcept override;

    /**
     * @brief Check whether the logic implementation was initialized correctly.
     *
     * @return True if the implementation was initialized correctly, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Run the system.
     *
     * @param[in] stop Reference to stop flag.
     */
    void run(const bool& stop) noexcept override;

    /**
     * @brief Handle button event.
     *
     *        Toggle the timer whenever the toggle button is pressed.
     *        Predict the temperature and restart the temperature timer whenever the temperature
     *        button is pressed.
     *
     *        Pin change interrupts are disabled for a debounce period after detecting button
     *        activity to mitigate the effects of contact bounce.
     */
    void handleButtonEvent() noexcept override;

    /**
     * @brief Handle debounce timer timerout.
     *
     *        Enable pin change interrupts after a debounce period following button activity to
     *        mitigate the effects of contact bounce.
     */
    void handleDebounceTimerTimeout() noexcept override;

    /**
     * @brief Handle toggle timer timeout.
     *
     *        Toggle the LED when the associated timer is enabled.
     */
    void handleToggleTimerTimeout() noexcept override;

    /**
     * @brief Handle temperature timer timeout.
     *
     *        Read the surrounding temperature.
     */
    void handleTempTimerTimeout() noexcept override;

    Logic()                        = delete; // No default constructor.
    Logic(const Logic&)            = delete; // No copy constructor.
    Logic(Logic&&)                 = delete; // No move constructor.
    Logic& operator=(const Logic&) = delete; // No copy assignment.
    Logic& operator=(Logic&&)      = delete; // No move assignment.

protected:
    driver::serial::Interface& serial() noexcept { return mySerial; }
    driver::eeprom::Interface& eeprom() noexcept { return myEeprom; }
    driver::tempsensor::Interface& tempSensor() noexcept { return myTempSensor; }
    static uint16_t toggleStateAddr() noexcept { return ToggleStateAddr; }

    virtual void writeToggleStateToEeprom(bool enable) noexcept;
    virtual bool readToggleStateFromEeprom() const noexcept;
    virtual void printTemperature() noexcept;

private:
    void handleToggleButtonPressed() noexcept;
    void handleTempButtonPressed() noexcept;
    void restoreToggleStateFromEeprom() noexcept;

    /** Toggle state address in EEPROM. */
    static constexpr uint16_t ToggleStateAddr{0U};

    /** Reference to the LED to toggle. */
    driver::gpio::Interface& myLed;

    /** Button to toggle the toggle timer. */
    driver::gpio::Interface& myToggleButton;

    /** Button to read the temperature. */
    driver::gpio::Interface& myTempButton;

    /** Debounce timer to mitigate effects of contact bounces. */
    driver::timer::Interface& myDebounceTimer;

    /** Timer to toggle the LED. */
    driver::timer::Interface& myToggleTimer;

    /** Timer to read the temperature. */
    driver::timer::Interface& myTempTimer;

    /** Serial device to print status messages. */
    driver::serial::Interface& mySerial;

    /** Watchdog timer that resets the program if it becomes unresponsive. */
    driver::watchdog::Interface& myWatchdog;

    /** EEPROM stream to write the status of the LED to EEPROM. */
    driver::eeprom::Interface& myEeprom;

    /** Temperature sensor. */
    driver::tempsensor::Interface& myTempSensor;
};
} // namespace logic