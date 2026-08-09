/**
 * @brief Generic logic implementation details for an MCU with configurable hardware devices.
 */
#include <stdint.h>

#include "driver/adc/interface.h"
#include "driver/eeprom/interface.h"
#include "driver/gpio/interface.h"
#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"
#include "logic/logic.h"

namespace logic
{
// -----------------------------------------------------------------------------
Logic::Logic(driver::gpio::Interface& led, driver::gpio::Interface& toggleButton,
             driver::gpio::Interface& tempButton, driver::timer::Interface& debounceTimer,
             driver::timer::Interface& toggleTimer, driver::timer::Interface& tempTimer,
             driver::serial::Interface& serial, driver::watchdog::Interface& watchdog,
             driver::eeprom::Interface& eeprom, driver::tempsensor::Interface& tempSensor) noexcept
    : myLed{led}
    , myToggleButton{toggleButton}
    , myTempButton{tempButton}
    , myDebounceTimer{debounceTimer}
    , myToggleTimer{toggleTimer}
    , myTempTimer{tempTimer}
    , mySerial{serial}
    , myWatchdog{watchdog}
    , myEeprom{eeprom}
    , myTempSensor{tempSensor}
{
    // Enable system if all hardware drivers were initialized correctly.
    if (isInitialized())
    {
        myToggleButton.enableInterrupt(true);
        myTempButton.enableInterrupt(true);
        myTempTimer.start();
        mySerial.setEnabled(true);
        myWatchdog.setEnabled(true);
        myEeprom.setEnabled(true);

        // Enable the toggle timer if it was enabled before poweroff.
        restoreToggleStateFromEeprom();
    }
}

// -----------------------------------------------------------------------------
Logic::~Logic() noexcept
{
    // Disable system.
    myLed.write(false);
    myToggleButton.enableInterrupt(false);
    myTempButton.enableInterrupt(false);
    myDebounceTimer.stop();
    myToggleTimer.stop();
    myTempTimer.stop();
    mySerial.setEnabled(false);
    myWatchdog.setEnabled(false);
    myEeprom.setEnabled(false);
    myToggleTimer.stop();
}

// -----------------------------------------------------------------------------
bool Logic::isInitialized() const noexcept
{
    // Return true if all hardware drivers are initialized.
    return myLed.isInitialized() && myToggleButton.isInitialized() &&
           myTempButton.isInitialized() && myDebounceTimer.isInitialized() &&
           myToggleTimer.isInitialized() && myTempTimer.isInitialized() &&
           mySerial.isInitialized() && myWatchdog.isInitialized() && myEeprom.isInitialized() &&
           myTempSensor.isInitialized();
}

// -----------------------------------------------------------------------------
void Logic::run(const bool& stop) noexcept
{
    // Terminate the function if initialization failed.
    if (!isInitialized())
    {
        // Print an error message is the serial device driver is working.
        if (mySerial.isInitialized())
        {
            const bool enabled{mySerial.isEnabled()};
            mySerial.setEnabled(true);
            mySerial.printf("Failed to run the system: initialization failed!\n");
            mySerial.setEnabled(enabled);
        }
        return;
    }

    // Run the system continuously.
    mySerial.printf("Running the system!\n");

    while (!stop)
    {
        // Regularly reset the watchdog to avoid system reset.
        myWatchdog.reset();
    }
}

// -----------------------------------------------------------------------------
void Logic::handleButtonEvent() noexcept
{
    // Ignore if this call was done manually.
    if (myDebounceTimer.isEnabled()) { return; }

    // Disable interrupts on the I/O ports to mitigate effects of debouncing.
    myToggleButton.enableInterruptOnPort(false);
    myTempButton.enableInterruptOnPort(false);
    myDebounceTimer.start();

    // Handle specific button event when pressed.
    if (myToggleButton.read()) { handleToggleButtonPressed(); }
    if (myTempButton.read()) { handleTempButtonPressed(); }
}

// -----------------------------------------------------------------------------
void Logic::handleDebounceTimerTimeout() noexcept
{
    // Re-enable interrupts on the ports after debounce timer timeout.
    if (myDebounceTimer.hasTimedOut())
    {
        myDebounceTimer.stop();
        myToggleButton.enableInterruptOnPort(true);
        myTempButton.enableInterruptOnPort(true);
    }
}

// -----------------------------------------------------------------------------
void Logic::handleToggleTimerTimeout() noexcept
{
    // Toggle the LED on toggle timer timeout.
    if (myToggleTimer.hasTimedOut()) { myLed.toggle(); }
}

// -----------------------------------------------------------------------------
void Logic::handleTempTimerTimeout() noexcept
{
    // Read and print the temperature on temperature timer timeout.
    if (myTempTimer.hasTimedOut()) { printTemperature(); }
}

// -----------------------------------------------------------------------------
void Logic::writeToggleStateToEeprom(const bool enable) noexcept
{
    myEeprom.write(ToggleStateAddr, static_cast<uint8_t>(enable));
}

// -----------------------------------------------------------------------------
bool Logic::readToggleStateFromEeprom() const noexcept
{
    uint8_t state{};
    return myEeprom.read(ToggleStateAddr, state) ? static_cast<bool>(state) : false;
}

// -----------------------------------------------------------------------------
void Logic::printTemperature() noexcept
{
    // Read and print the temperature.
    const int16_t temperature{myTempSensor.read()};
    mySerial.printf("Temperature: %d Celsius\n", temperature);
}

// -----------------------------------------------------------------------------
void Logic::handleToggleButtonPressed() noexcept
{
    // Toggle the toggle timer on pressdown, safe the current LED state in EEPROM.
    myToggleTimer.toggle();
    writeToggleStateToEeprom(myToggleTimer.isEnabled());

    if (myToggleTimer.isEnabled()) { mySerial.printf("Toggle timer enabled!\n"); }
    else
    {
        // Immediately disable the LED if the toggle timer is disabled to ensure that the LED
        // isn't stuck in an enabled state.
        mySerial.printf("Toggle timer disabled!\n");
        myLed.write(false);
    }
}

// -----------------------------------------------------------------------------
void Logic::handleTempButtonPressed() noexcept
{
    // Read and print the temperature on pressdown.
    // Restart the temperature timer.
    printTemperature();
    myTempTimer.restart();
}

// -----------------------------------------------------------------------------
void Logic::restoreToggleStateFromEeprom() noexcept
{
    // Start the toggle timer if the LED was enabled before poweroff.
    if (readToggleStateFromEeprom())
    {
        myToggleTimer.start();
        mySerial.printf("Toggle timer enabled!\n");
    }
}
} // namespace logic