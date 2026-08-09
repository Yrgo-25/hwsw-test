/**
 * @brief Test hardware platform for AVR devices.
 */
#ifdef TESTSUITE

#pragma once

#include <cstdint>
#include <string>

namespace test
{
/**
 * @brief Register memory union to access data as bytes and words.
 *
 * @tparam ByteCount Size of the memory in bytes.
 */
template<std::size_t ByteCount>
union RegisterMemory
{
    /** Array representing 8-bit registers. */
    std::uint8_t reg8[ByteCount];

    /** Array representing 16-bit registers. */
    std::uint16_t reg16[ByteCount / 2U];
};

/**
 * @brief Memory structure for testing.
 */
struct Memory
{
    /** Size of the memory in bytes. */
    static constexpr std::size_t Size{256U};

    /** Array representing registers. */
    static RegisterMemory<Size> data;
};

/**
 * @brief Execute assembly command.
 *
 * @param[in] cmd The command to execute.
 */
void executeAssemblyCmd(const std::string& cmd) noexcept;

/**
 * @brief Generate delay in ms.
 *
 * @param[in] ms The delay duration in ms.
 */
void delay_ms(std::uint16_t ms) noexcept;

/**
 * @brief Generate delay in us.
 *
 * @param[in] ms The delay duration in us.
 */
void delay_us(std::uint16_t us) noexcept;

} // namespace test

/** Mapping of AVR registers. */
#define SREG test::Memory::data.reg8[0U]
#define MCUSR test::Memory::data.reg8[1U]
#define DDRB test::Memory::data.reg8[2U]
#define DDRC test::Memory::data.reg8[3U]
#define DDRD test::Memory::data.reg8[4U]
#define PORTB test::Memory::data.reg8[5U]
#define PORTC test::Memory::data.reg8[6U]
#define PORTD test::Memory::data.reg8[7U]
#define PINB test::Memory::data.reg8[8U]
#define PINC test::Memory::data.reg8[9U]
#define PIND test::Memory::data.reg8[10U]
#define PCMSK0 test::Memory::data.reg8[11U]
#define PCMSK1 test::Memory::data.reg8[12U]
#define PCMSK2 test::Memory::data.reg8[13U]
#define PCIE0 test::Memory::data.reg8[14U]
#define PCIE1 test::Memory::data.reg8[15U]
#define PCIE2 test::Memory::data.reg8[16U]

#define TCCR0A test::Memory::data.reg8[17U]
#define TCCR0B test::Memory::data.reg8[18U]
#define TCNT0 test::Memory::data.reg8[19U]
#define OCR0A test::Memory::data.reg8[20U]
#define OCR0B test::Memory::data.reg8[21U]
#define TIMSK0 test::Memory::data.reg8[22U]
#define TIFR0 test::Memory::data.reg8[23U]

#define TCCR1A test::Memory::data.reg8[24U]
#define TCCR1B test::Memory::data.reg8[25U]
#define TCCR1C test::Memory::data.reg8[26U]
#define OCR1AL test::Memory::data.reg8[30U]
#define OCR1AH test::Memory::data.reg8[31U]
#define OCR1A test::Memory::data.reg16[15U]
#define TIMSK1 test::Memory::data.reg8[35U]
#define TIFR1 test::Memory::data.reg8[36U]
// The remaining 16-bit Timer1 registers (TCNT1, OCR1B, ICR1) are defined further down,
// together with their high/low byte halves.

#define TCCR2A test::Memory::data.reg8[37U]
#define TCCR2B test::Memory::data.reg8[38U]
#define TCNT2 test::Memory::data.reg8[39U]
#define OCR2A test::Memory::data.reg8[40U]
#define OCR2B test::Memory::data.reg8[41U]
#define TIMSK2 test::Memory::data.reg8[42U]
#define TIFR2 test::Memory::data.reg8[43U]

#define SPCR test::Memory::data.reg8[44U]
#define SPSR test::Memory::data.reg8[45U]
#define SPDR test::Memory::data.reg8[46U]

#define UCSR0A test::Memory::data.reg8[47U]
#define UCSR0B test::Memory::data.reg8[48U]
#define UCSR0C test::Memory::data.reg8[49U]
#define UBRR0 test::Memory::data.reg16[25U]
#define UBRR0L test::Memory::data.reg8[50U]
#define UBRR0H test::Memory::data.reg8[51U]
#define UDR0 test::Memory::data.reg8[52U]

#define ADMUX test::Memory::data.reg8[53U]
#define ADCSRA test::Memory::data.reg8[54U]
#define ADCSRB test::Memory::data.reg8[55U]
#define ADCL test::Memory::data.reg8[56U]
#define ADCH test::Memory::data.reg8[57U]
#define ADC test::Memory::data.reg16[28U]
#define DIDR0 test::Memory::data.reg8[58U]
#define DIDR1 test::Memory::data.reg8[59U]

#define EIMSK test::Memory::data.reg8[60U]
#define EIFR test::Memory::data.reg8[61U]
#define EICRA test::Memory::data.reg8[62U]
#define PCICR test::Memory::data.reg8[63U]
#define PCIFR test::Memory::data.reg8[64U]

#define GPIOR0 test::Memory::data.reg8[65U]
#define GPIOR1 test::Memory::data.reg8[66U]
#define GPIOR2 test::Memory::data.reg8[67U]
#define PRR test::Memory::data.reg8[68U]
#define CLKPR test::Memory::data.reg8[69U]
#define WDTCSR test::Memory::data.reg8[70U]
#define SMCR test::Memory::data.reg8[71U]
#define SPMCSR test::Memory::data.reg8[72U]

#define DDRA test::Memory::data.reg8[73U]
#define DDRF test::Memory::data.reg8[74U]
#define DDRG test::Memory::data.reg8[75U]
#define DDRH test::Memory::data.reg8[76U]
#define DDRJ test::Memory::data.reg8[77U]
#define DDRK test::Memory::data.reg8[78U]
#define DDRL test::Memory::data.reg8[79U]
#define DDRE test::Memory::data.reg8[80U]

#define PORTA test::Memory::data.reg8[81U]
#define PORTF test::Memory::data.reg8[82U]
#define PORTG test::Memory::data.reg8[83U]
#define PORTH test::Memory::data.reg8[84U]
#define PORTJ test::Memory::data.reg8[85U]
#define PORTK test::Memory::data.reg8[86U]
#define PORTL test::Memory::data.reg8[87U]
#define PORTE test::Memory::data.reg8[88U]

#define PINA test::Memory::data.reg8[89U]
#define PINF test::Memory::data.reg8[90U]
#define PING test::Memory::data.reg8[91U]
#define PINH test::Memory::data.reg8[92U]
#define PINJ test::Memory::data.reg8[93U]
#define PINK test::Memory::data.reg8[94U]
#define PINL test::Memory::data.reg8[95U]
#define PINE test::Memory::data.reg8[96U]

#define TCCR3A test::Memory::data.reg8[97U]
#define TCCR3B test::Memory::data.reg8[98U]
#define TCCR3C test::Memory::data.reg8[99U]
#define TIMSK3 test::Memory::data.reg8[110U]
#define TIFR3 test::Memory::data.reg8[111U]

#define TCCR4A test::Memory::data.reg8[112U]
#define TCCR4B test::Memory::data.reg8[113U]
#define TCCR4C test::Memory::data.reg8[114U]
#define TIMSK4 test::Memory::data.reg8[125U]
#define TIFR4 test::Memory::data.reg8[126U]

#define TCCR5A test::Memory::data.reg8[127U]
#define TCCR5B test::Memory::data.reg8[128U]
#define TCCR5C test::Memory::data.reg8[129U]
#define TIMSK5 test::Memory::data.reg8[140U]
#define TIFR5 test::Memory::data.reg8[141U]

#define UCSR1A test::Memory::data.reg8[142U]
#define UCSR1B test::Memory::data.reg8[143U]
#define UCSR1C test::Memory::data.reg8[144U]
#define UBRR1H test::Memory::data.reg8[145U]
#define UBRR1L test::Memory::data.reg8[146U]
#define UDR1 test::Memory::data.reg8[147U]

#define UCSR2A test::Memory::data.reg8[148U]
#define UCSR2B test::Memory::data.reg8[149U]
#define UCSR2C test::Memory::data.reg8[150U]
#define UBRR2H test::Memory::data.reg8[151U]
#define UBRR2L test::Memory::data.reg8[152U]
#define UDR2 test::Memory::data.reg8[153U]

#define UCSR3A test::Memory::data.reg8[154U]
#define UCSR3B test::Memory::data.reg8[155U]
#define UCSR3C test::Memory::data.reg8[156U]
#define UBRR3H test::Memory::data.reg8[157U]
#define UBRR3L test::Memory::data.reg8[158U]
#define UDR3 test::Memory::data.reg8[159U]

#define EECR test::Memory::data.reg8[160U]
#define EEDR test::Memory::data.reg8[161U]
#define EEAR test::Memory::data.reg16[82U]

/**
 * 16-bit timer registers, each paired with its own high/low byte halves.
 *
 * reg16[n] occupies bytes reg8[2n] and reg8[2n + 1]. On a little-endian host the low byte is
 * the even index and the high byte the odd one, so writing the 16-bit register is visible
 * through its low/high byte macros and vice versa - exactly as on real hardware. Keep that
 * invariant when adding registers here, otherwise a driver writing OCR1B would leave OCR1BL
 * unchanged.
 */
#define ICR1L test::Memory::data.reg8[166U]
#define ICR1H test::Memory::data.reg8[167U]
#define ICR1 test::Memory::data.reg16[83U]

#define TCNT1L test::Memory::data.reg8[168U]
#define TCNT1H test::Memory::data.reg8[169U]
#define TCNT1 test::Memory::data.reg16[84U]

#define OCR1BL test::Memory::data.reg8[170U]
#define OCR1BH test::Memory::data.reg8[171U]
#define OCR1B test::Memory::data.reg16[85U]

#define OCR3AL test::Memory::data.reg8[172U]
#define OCR3AH test::Memory::data.reg8[173U]
#define OCR3A test::Memory::data.reg16[86U]

#define ICR3L test::Memory::data.reg8[174U]
#define ICR3H test::Memory::data.reg8[175U]
#define ICR3 test::Memory::data.reg16[87U]

#define TCNT3L test::Memory::data.reg8[176U]
#define TCNT3H test::Memory::data.reg8[177U]
#define TCNT3 test::Memory::data.reg16[88U]

#define OCR3BL test::Memory::data.reg8[178U]
#define OCR3BH test::Memory::data.reg8[179U]
#define OCR3B test::Memory::data.reg16[89U]

#define OCR4AL test::Memory::data.reg8[180U]
#define OCR4AH test::Memory::data.reg8[181U]
#define OCR4A test::Memory::data.reg16[90U]

#define ICR4L test::Memory::data.reg8[182U]
#define ICR4H test::Memory::data.reg8[183U]
#define ICR4 test::Memory::data.reg16[91U]

#define TCNT4L test::Memory::data.reg8[184U]
#define TCNT4H test::Memory::data.reg8[185U]
#define TCNT4 test::Memory::data.reg16[92U]

#define OCR4BL test::Memory::data.reg8[186U]
#define OCR4BH test::Memory::data.reg8[187U]
#define OCR4B test::Memory::data.reg16[93U]

#define OCR5AL test::Memory::data.reg8[188U]
#define OCR5AH test::Memory::data.reg8[189U]
#define OCR5A test::Memory::data.reg16[94U]

#define ICR5L test::Memory::data.reg8[190U]
#define ICR5H test::Memory::data.reg8[191U]
#define ICR5 test::Memory::data.reg16[95U]

#define TCNT5L test::Memory::data.reg8[192U]
#define TCNT5H test::Memory::data.reg8[193U]
#define TCNT5 test::Memory::data.reg16[96U]

#define OCR5BL test::Memory::data.reg8[194U]
#define OCR5BH test::Memory::data.reg8[195U]
#define OCR5B test::Memory::data.reg16[97U]

#define OCR3CL test::Memory::data.reg8[196U]
#define OCR3CH test::Memory::data.reg8[197U]
#define OCR3C test::Memory::data.reg16[98U]

#define OCR4CL test::Memory::data.reg8[198U]
#define OCR4CH test::Memory::data.reg8[199U]
#define OCR4C test::Memory::data.reg16[99U]

#define OCR5CL test::Memory::data.reg8[200U]
#define OCR5CH test::Memory::data.reg8[201U]
#define OCR5C test::Memory::data.reg16[100U]

/** Mapping of AVR register bits and flags. */
#define I_FLAG 7U
#define WDP0 0U
#define WDP1 1U
#define WDP2 2U
#define WDP3 5U
#define WDCE 4U
#define WDE 3U
#define WDRF 3U

#define REFS0 6U
#define ADEN 7U
#define ADSC 6U
#define ADPS0 0U
#define ADPS1 1U
#define ADPS2 2U
#define ADIF 4U

#define CS01 1U
#define CS11 1U
#define CS21 1U
#define WGM12 3U
#define TOIE0 0U
#define OCIE1A 1U
#define TOIE2 0U

#define UDRE0 5U
#define RXEN0 4U
#define TXEN0 3U
#define UCSZ00 1U
#define UCSZ01 2U
#define RXC0 7U

#define EEPE 1U
#define EEMPE 2U
#define EERE 0U

/** Execute an assembly command. */
#define asm(cmd) test::executeAssemblyCmd(cmd)

/** Generate delay in ms. */
#define _delay_ms(ms) test::delay_ms(ms);

/** Generate delay in us. */
#define _delay_us(us) test::delay_us(us)

/** Implement interrupt service routines as functions. */
#define ISR(vector) void vector() noexcept

#endif /** TESTSUITE */