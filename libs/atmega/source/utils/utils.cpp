/**
 * @brief Implementation details of miscellaneous utility functions.
 */
#include <stdint.h>

#include "arch/avr/hw_platform.h"
#include "utils/utils.h"

namespace utils
{
// -----------------------------------------------------------------------------
void delay_s(const uint16_t& delayTime_s) noexcept
{
    for (uint16_t i{}; i < delayTime_s; ++i)
    {
        _delay_ms(1000U);
    }
}

// -----------------------------------------------------------------------------
void delay_ms(const uint16_t& delayTime_ms) noexcept
{
    for (uint16_t i{}; i < delayTime_ms; ++i)
    {
        _delay_ms(1U);
    }
}

// -----------------------------------------------------------------------------
void delay_us(const uint16_t& delayTime_us) noexcept
{
    for (uint16_t i{}; i < delayTime_us; ++i)
    {
        _delay_us(1U);
    }
}

// -----------------------------------------------------------------------------
void globalInterruptEnable() noexcept { asm("SEI"); }

// -----------------------------------------------------------------------------
void globalInterruptDisable() noexcept { asm("CLI"); }

} // namespace utils

/**
 * @note Definitions required to satisfy the linker when using C++ interfaces and static local
 *       variables in embedded projects.
 */
void operator delete(void*, unsigned int) noexcept {}
extern "C" void __cxa_pure_virtual() {}
extern "C" int __cxa_guard_acquire(volatile void* g) { return !*(char*)g; }
extern "C" void __cxa_guard_release(volatile void* g) { *(char*)g = 1; }
extern "C" void __cxa_guard_abort(volatile void*) {}