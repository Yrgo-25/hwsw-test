/**
 * @brief Implementation details of hardware timer driver.
 */
#include <stdint.h>

#include "arch/avr/hw_platform.h"
#include "container/array.h"
#include "driver/timer/atmega328p.h"
#include "utils/callback_array.h"
#include "utils/utils.h"

namespace driver
{
namespace timer
{
/**
 * @brief Structure for implementation of timer hardware.
 */
struct Atmega328p::Hardware
{
    /** Hardware counter. */
    volatile uint32_t counter;

    /** Pointer to mask register. */
    volatile uint8_t* maskReg;

    /** Mask bit for timer interrupt. */
    uint8_t maskBit;

    /** Timer index. */
    uint8_t index;

    static Hardware* reserve() noexcept;
    static void release(Hardware* hw) noexcept;

private:
    static Hardware* init(const uint8_t timerIndex) noexcept;
};

namespace
{
using container::CallbackArray;

/**
 * @brief Structure containing indexes for available timer circuits.
 */
struct Index
{
    /** Index for Timer 0. */
    static constexpr uint8_t Timer0{0U};

    /** Index for Timer 1. */
    static constexpr uint8_t Timer1{1U};

    /** Index for Timer 2. */
    static constexpr uint8_t Timer2{2U};
};

/** The number of available timer circuits. */
constexpr uint8_t CircuitCount{3U};

/** Time between each timer interrupt in ms. */
constexpr double InterruptIntervalMs{0.128};

/** Array holding pointers to timers. */
Atmega328p* myTimers[CircuitCount]{};

/** Array holding pointers to callbacks. */
CallbackArray<CircuitCount> myCallbacks{};

// -----------------------------------------------------------------------------
constexpr uint32_t maxCount(const uint32_t timeout_ms) noexcept
{
    return 0U < timeout_ms ? utils::round<uint32_t>(timeout_ms / InterruptIntervalMs) : 0U;
}

// -----------------------------------------------------------------------------
void invokeCallback(const uint8_t timerIndex) noexcept
{
    // Check the timer index, terminate the function if invalid.
    if (CircuitCount <= timerIndex) { return; }

    // Invoke callback.
    Atmega328p* timer{myTimers[timerIndex]};
    if (nullptr != timer) { timer->handleCallback(); }
}
} // namespace

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p(const uint32_t timeout_ms, void (*callback)(),
                       const bool startTimer) noexcept
    : myHw{0U < timeout_ms ? Hardware::reserve() : nullptr}
    , myMaxCount{maxCount(timeout_ms)}
    , myEnabled{false}
{
    if (nullptr == myHw) { return; }
    myTimers[myHw->index] = this;
    addCallback(callback);
    if (startTimer) { start(); }
}

// -----------------------------------------------------------------------------
Atmega328p::~Atmega328p() noexcept
{
    // Do nothing if the circuit isn't initialized (nothing to clean up).
    if (!isInitialized()) { return; }
    removeCallback();
    myTimers[myHw->index] = nullptr;
    Hardware::release(myHw);
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return nullptr != myHw; }

// -----------------------------------------------------------------------------
bool Atmega328p::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
bool Atmega328p::hasTimedOut() const noexcept { return myEnabled && (myHw->counter >= myMaxCount); }

// -----------------------------------------------------------------------------
uint32_t Atmega328p::timeout_ms() const noexcept
{
    return utils::round<uint32_t>(myMaxCount * InterruptIntervalMs);
}

// -----------------------------------------------------------------------------
void Atmega328p::setTimeout_ms(const uint32_t timeout_ms) noexcept
{
    // Do nothing if the timeout is invalid.
    if (0U == timeout_ms) { return; }
    myMaxCount = maxCount(timeout_ms);
}

// -----------------------------------------------------------------------------
void Atmega328p::start() noexcept
{
    if (0U == myMaxCount) { return; }
    utils::globalInterruptEnable();
    utils::set(*(myHw->maskReg), myHw->maskBit);
    myEnabled = true;
}

// -----------------------------------------------------------------------------
void Atmega328p::stop() noexcept
{
    *(myHw->maskReg) = 0U;
    myEnabled        = false;
}

// -----------------------------------------------------------------------------
void Atmega328p::toggle() noexcept
{
    if (myEnabled) { stop(); }
    else { start(); }
}

// -----------------------------------------------------------------------------
void Atmega328p::restart() noexcept
{
    myHw->counter = 0U;
    start();
}

// -----------------------------------------------------------------------------
void Atmega328p::handleCallback() noexcept
{
    // Increment the timer, invoke callback on timeout.
    increment();

    if (hasTimedOut())
    {
        myCallbacks.invoke(myHw->index);
        clearTimedOut();
    }
}

// -----------------------------------------------------------------------------
void Atmega328p::addCallback(void (*callback)()) const noexcept
{
    myCallbacks.add(callback, myHw->index);
}

// -----------------------------------------------------------------------------
void Atmega328p::removeCallback() const noexcept { myCallbacks.remove(myHw->index); }

// -----------------------------------------------------------------------------
bool Atmega328p::increment() noexcept
{
    if (!myEnabled) { return false; }
    myHw->counter++;
    return true;
}

// -----------------------------------------------------------------------------
void Atmega328p::clearTimedOut() noexcept { myHw->counter = 0U; }

// -----------------------------------------------------------------------------
Atmega328p::Hardware* Atmega328p::Hardware::reserve() noexcept
{
    // Reserve a timer circuit if any is available, otherwise return a nullptr.
    for (uint8_t i{}; i < CircuitCount; ++i)
    {
        if (nullptr == myTimers[i]) { return init(i); }
    }
    return nullptr;
}

// -----------------------------------------------------------------------------
void Atmega328p::Hardware::release(Atmega328p::Hardware* hw) noexcept
{
    // Reset the associated hardware timer.
    *(hw->maskReg) = 0U;

    switch (hw->index)
    {
        case Index::Timer0:
            TCCR0B = 0U;
            break;
        case Index::Timer1:
            TCCR1B = 0U;
            OCR1A  = 0U;
            break;
        case Index::Timer2:
            TCCR2B = 0U;
            break;
        default:
            break;
    }
    // Release allocated resources.
    utils::deleteMemory(hw);
}

// -----------------------------------------------------------------------------
Atmega328p::Hardware* Atmega328p::Hardware::init(const uint8_t timerIndex) noexcept
{
    constexpr uint16_t timer1MaxCount{256U};
    constexpr uint8_t controlBits0{(1U << CS01)};
    constexpr uint8_t controlBits1{(1U << CS11) | (1U << WGM12)};
    constexpr uint8_t controlBits2{(1U << CS21)};

    // Allocate memory for the new timer hardware, return false is memory allocation failed.
    Hardware* hw{utils::newMemory<Hardware>()};
    if (nullptr == hw) { return nullptr; }

    // Set the structure to refer to the corresponding timer circuit.
    switch (timerIndex)
    {
        case Index::Timer0:
            hw->maskReg = &TIMSK0;
            hw->maskBit = TOIE0;
            TCCR0B      = controlBits0;
            break;
        case Index::Timer1:
            hw->maskReg = &TIMSK1;
            hw->maskBit = OCIE1A;
            TCCR1B      = controlBits1;
            OCR1A       = timer1MaxCount;
            break;
        case Index::Timer2:
            hw->maskReg = &TIMSK2;
            hw->maskBit = TOIE2;
            TCCR2B      = controlBits2;
            break;
        default:
            utils::deleteMemory(hw);
            return nullptr;
    }
    // Return the initialized circuit.
    hw->counter = 0U;
    hw->index   = timerIndex;
    return hw;
}

// -----------------------------------------------------------------------------
ISR(TIMER0_OVF_vect) { invokeCallback(Index::Timer0); }

// -----------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect) { invokeCallback(Index::Timer1); }

// -----------------------------------------------------------------------------
ISR(TIMER2_OVF_vect) { invokeCallback(Index::Timer2); }

} // namespace timer
} // namespace driver
