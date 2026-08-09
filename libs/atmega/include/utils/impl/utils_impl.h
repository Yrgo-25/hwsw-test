/**
 * @brief Implementation details of miscellaneous utility functions.
 *
 * @note Don't include this header, use <utils.h> instead!
 */
#pragma once

namespace utils
{
// -----------------------------------------------------------------------------
template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type& object) noexcept
{
    return static_cast<T&&>(object);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type&& object) noexcept
{
    return static_cast<T&&>(object);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr void set(volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg |= (1ULL << bit);
}

// -----------------------------------------------------------------------------
template<typename T, typename... Bits>
constexpr void set(volatile T& reg, const uint8_t bit, Bits&&... bits) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    set(reg, bit);
    set(reg, forward<const Bits>(bits)...);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr void clear(volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg &= ~(1ULL << bit);
}

// -----------------------------------------------------------------------------
template<typename T, typename... Bits>
constexpr void clear(volatile T& reg, const uint8_t bit, Bits&&... bits) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    clear(reg, bit);
    clear(reg, forward<const Bits>(bits)...);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr void toggle(volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg ^= (1ULL << bit);
}

// -----------------------------------------------------------------------------
template<typename T, typename... Bits>
constexpr void toggle(volatile T& reg, const uint8_t bit, Bits&&... bits) noexcept
{
    toggle(reg, bit);
    toggle(reg, forward<const Bits>(bits)...);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr bool read(const volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    return reg & (1ULL << bit);
}

// -----------------------------------------------------------------------------
template<typename T, typename... Bits>
constexpr bool read(const volatile T& reg, const uint8_t bit, Bits&&... bits) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    return read(reg, bit) | read(reg, forward<const Bits>(bits)...);
}

// -----------------------------------------------------------------------------
template<typename T1, typename T2, typename T3>
T3 power(const T1 base, const T2 exponent) noexcept
{
    static_assert(type_traits::is_arithmetic<T1>::value && type_traits::is_arithmetic<T2>::value &&
                      type_traits::is_arithmetic<T3>::value,
                  "Calculation of power only possible for arithmetic types!");
    T1 num{1};
    for (uint16_t i{}; i < exponent; ++i)
    {
        num *= base;
    }
    return static_cast<T3>(num);
}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
constexpr T1 round(const T2 number) noexcept
{
    static_assert(type_traits::is_integral<T1>::value && type_traits::is_arithmetic<T2>::value,
                  "Rounding only possible for arithmetic types!");
    return static_cast<T1>(0.0 <= number ? number + 0.5 : number - 0.5);
}

// -----------------------------------------------------------------------------
template<typename T>
constexpr bool inRange(const T number, const T min, const T max) noexcept
{
    static_assert(type_traits::is_arithmetic<T>::value,
                  "Range checks are only supported for arithmetic types!");
    return ((min <= number) && (max >= number));
}

// -----------------------------------------------------------------------------
template<typename T, typename... Args>
inline T* newObject(Args&&... args) noexcept
{
    auto block{newMemory<T>()};
    if (block) { *block = T{forward<Args>(args)...}; }
    return block;
}

// -----------------------------------------------------------------------------
template<typename T>
inline T* newMemory(const size_t size) noexcept
{
    return static_cast<T*>(malloc(sizeof(T) * size));
}

// -----------------------------------------------------------------------------
template<typename T>
inline T* reallocMemory(T* block, const size_t newSize) noexcept
{
    return static_cast<T*>(realloc(block, sizeof(T) * newSize));
}

// -----------------------------------------------------------------------------
template<typename T>
inline void deleteMemory(T*& block) noexcept
{
    free(block);
    block = nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
inline T move(T&& source) noexcept
{
    T copy{source};
    source = {};
    return copy;
}
} // namespace utils