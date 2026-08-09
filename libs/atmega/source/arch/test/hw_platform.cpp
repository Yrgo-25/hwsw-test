/**
 * @brief Test hardware platform implementation details for AVR devices.
 */
#ifdef TESTSUITE

#include <chrono>
#include <cstdint>
#include <string>
#include <thread>

#include "arch/test/hw_platform.h"

/** Set bit in a register. */
#define SET(reg, bit) ((reg) |= (1ULL << (bit)))

/** Clear bit in a register. */
#define CLR(reg, bit) ((reg) &= ~(1ULL << (bit)))

/** Read bit in a register. */
#define READ(reg, bit) static_cast<bool>((reg) & (1ULL << bit))

namespace test
{
/** Array representing registers. */
RegisterMemory<Memory::Size> Memory::data{};

// -----------------------------------------------------------------------------
void executeAssemblyCmd(const std::string& cmd) noexcept
{
    if ("SEI" == cmd) { SET(SREG, I_FLAG); }
    else if ("CLI" == cmd) { CLR(SREG, I_FLAG); }
    // No-op: watchdog counter reset not needed in unit tests.
    else if ("WDR" == cmd) {}
}

// -----------------------------------------------------------------------------
void delay_ms(const std::uint16_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
void delay_us(const std::uint16_t us) noexcept
{
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}
} // namespace test

#endif /** TESTSUITE */