/**
 * @brief Firmware entry point for ATmega328P.
 */
#include <stdint.h>

#include "driver/adc/atmega328p.h"
#include "driver/eeprom/atmega328p.h"
#include "driver/gpio/atmega328p.h"
#include "driver/serial/atmega328p.h"
#include "driver/tempsensor/tmp36.h"
#include "driver/timer/atmega328p.h"
#include "driver/watchdog/atmega328p.h"
#include "logic/logic.h"

namespace
{
/** Pointer to the logic implementation, used by the interrupt callbacks. */
logic::Interface* myLogic{nullptr};

// -----------------------------------------------------------------------------
void handleButtonEvent() noexcept
{
    if (nullptr != myLogic) { myLogic->handleButtonEvent(); }
}

// -----------------------------------------------------------------------------
void handleDebounceTimerTimeout() noexcept
{
    if (nullptr != myLogic) { myLogic->handleDebounceTimerTimeout(); }
}

// -----------------------------------------------------------------------------
void handleToggleTimerTimeout() noexcept
{
    if (nullptr != myLogic) { myLogic->handleToggleTimerTimeout(); }
}

// -----------------------------------------------------------------------------
void handleTempTimerTimeout() noexcept
{
    if (nullptr != myLogic) { myLogic->handleTempTimerTimeout(); }
}
} // namespace

/**
 * @brief Create the drivers and run the system.
 *
 * @return 0 on termination of the program (should never occur).
 */
int main()
{
    using namespace driver;

    // Pin configuration.
    constexpr uint8_t ledPin{gpio::Atmega328p::Port::B0};
    constexpr uint8_t toggleButtonPin{gpio::Atmega328p::Port::B4};
    constexpr uint8_t tempButtonPin{gpio::Atmega328p::Port::B5};
    constexpr uint8_t tempSensorPin{2U}; // Analog pin A2.

    // Timer configuration.
    constexpr uint32_t debounceTimeout_ms{300U};
    constexpr uint32_t toggleTimeout_ms{100U};
    constexpr uint32_t tempTimeout_ms{60000U};

    // Create the hardware drivers.
    gpio::Atmega328p led{ledPin, gpio::Mode::Output};
    gpio::Atmega328p toggleButton{toggleButtonPin, gpio::Mode::InputPullup, handleButtonEvent};
    gpio::Atmega328p tempButton{tempButtonPin, gpio::Mode::InputPullup, handleButtonEvent};
    timer::Atmega328p debounceTimer{debounceTimeout_ms, handleDebounceTimerTimeout};
    timer::Atmega328p toggleTimer{toggleTimeout_ms, handleToggleTimerTimeout};
    timer::Atmega328p tempTimer{tempTimeout_ms, handleTempTimerTimeout};
    tempsensor::Tmp36 tempSensor{tempSensorPin, adc::Atmega328p::getInstance()};

    // Create the logic implementation and connect it to the interrupt callbacks.
    logic::Logic logic{led,
                       toggleButton,
                       tempButton,
                       debounceTimer,
                       toggleTimer,
                       tempTimer,
                       serial::Atmega328p::getInstance(),
                       watchdog::Atmega328p::getInstance(),
                       eeprom::Atmega328p::getInstance(),
                       tempSensor};
    myLogic = &logic;

    // Run the system continuously.
    constexpr bool stop{false};
    logic.run(stop);
    return 0;
}
