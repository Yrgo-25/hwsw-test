/**
 * @brief GPIO driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/gpio/interface.h"

namespace driver
{
namespace gpio
{
/** GPIO hardware structure. */
struct Hardware;

/**
 * @brief GPIO driver for ATmega328P.
 *
 *        This class is non-copyable and non-movable.
 */
class Atmega328p final : public Interface
{
public:
    /** Port aliases for GPIO pins. */
    struct Port;

    /** Enumeration of I/O ports. */
    enum class IoPort : uint8_t;

    /**
     * @brief Constructor.
     *
     * @param[in] pin The pin number of the GPIO.
     * @param[in] mode GPIO mode.
     * @param[in] callback Callback associated with the GPIO (default = none).
     */
    explicit Atmega328p(uint8_t pin, Mode mode, void (*callback)() = nullptr) noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept override;

    /**
     * @brief Check whether the GPIO is initialized.
     *
     *        An uninitialized GPIO indicates that the specified PIN was unavailable or invalid
     *        when the GPIO was created.
     *
     * @return True if the GPIO is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Get the configured mode of the GPIO.
     *
     * @return The configured mode of the GPIO.
     */
    Mode mode() const noexcept override;

    /**
     * @brief Read input of the GPIO.
     *
     * @return True if the input is high, false otherwise.
     */
    bool read() const noexcept override;

    /**
     * @brief Write output to the GPIO.
     *
     * @param[in] output The output value to write (true = high, false = low).
     */
    void write(bool output) noexcept override;

    /**
     * @brief Toggle the output of the GPIO.
     *
     * @note This operation is only supported for pins set to output.
     */
    void toggle() noexcept override;

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the GPIO, false otherwise.
     */
    void enableInterrupt(bool enable) noexcept override;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     *
     * @param[in] enable True to enable pin change interrupt for the I/O port, false otherwise.
     */
    void enableInterruptOnPort(bool enable) noexcept override;

    /**
     * @brief Blink output of the GPIO with the given blink speed.
     *
     * @param[in] blinkSpeedMs The blink speed in milliseconds.
     *
     * @note This operation is only supported for pins set to output.
     */
    void blink(const uint16_t& blinkSpeed_ms) noexcept;

    Atmega328p()                             = delete; // No default constructor.
    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    IoPort getIoPort(uint8_t id) const noexcept;
    uint8_t getPhysicalPin() const noexcept;
    bool initHw() noexcept;

    /** Hardware structure associated with the GPIO. */
    Hardware* myHw;

    /** Configured mode. */
    Mode myMode;

    /** Associated I/O port. */
    const IoPort myIoPort;

    /** GPIO ID. */
    uint8_t myId;

    /** Pin the GPIO is connected to. */
    uint8_t myPin;
};

/**
 * @brief Port aliases for GPIO pins.
 *
 *        - Pins 0 - 7 are associated with I/O port D.
 *
 *        - Pins 8 - 13 are associated with I/O port B.
 *
 *        - Pins 14 - 19 are associated with I/O port C.
 */
struct Atmega328p::Port
{
    static constexpr uint8_t D0{0}; // PORTD0 = pin 0.
    static constexpr uint8_t D1{1}; // PORTD1 = pin 1.
    static constexpr uint8_t D2{2}; // PORTD2 = pin 2.
    static constexpr uint8_t D3{3}; // PORTD3 = pin 3.
    static constexpr uint8_t D4{4}; // PORTD4 = pin 4.
    static constexpr uint8_t D5{5}; // PORTD5 = pin 5.
    static constexpr uint8_t D6{6}; // PORTD6 = pin 6.
    static constexpr uint8_t D7{7}; // PORTD7 = pin 7.

    static constexpr uint8_t B0{8};  // PORTB0 = pin 8.
    static constexpr uint8_t B1{9};  // PORTB1 = pin 9.
    static constexpr uint8_t B2{10}; // PORTB2 = pin 10.
    static constexpr uint8_t B3{11}; // PORTB3 = pin 11.
    static constexpr uint8_t B4{12}; // PORTB4 = pin 12.
    static constexpr uint8_t B5{13}; // PORTB5 = pin 13.

    static constexpr uint8_t C0{14}; // PORTC0 = pin 14.
    static constexpr uint8_t C1{15}; // PORTC1 = pin 15.
    static constexpr uint8_t C2{16}; // PORTC2 = pin 16.
    static constexpr uint8_t C3{17}; // PORTC3 = pin 17.
    static constexpr uint8_t C4{18}; // PORTC4 = pin 18.
    static constexpr uint8_t C5{19}; // PORTC5 = pin 10.
};

/**
 * @brief Enumeration of I/O ports.
 */
enum class Atmega328p::IoPort : uint8_t
{
    B,     // I/O port B.
    C,     // I/O port C.
    D,     // I/O port D.
    Count, // The number of I/O ports available.
};
} // namespace gpio
} // namespace driver
