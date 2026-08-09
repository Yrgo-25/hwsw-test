/**
 * @brief Contains miscellaneous utility functions for bit operations,
 *        mathematical operations and generating delays.
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/type_traits.h"

namespace utils
{
/**
 * @brief Block the calling thread for the given time measured in seconds.
 *
 * @param[in] delayTime_s The time to block the thread in seconds.
 */
void delay_s(const uint16_t& delayTime_s) noexcept;

/**
 * @brief Block the calling thread for the given time measured in milliseconds.
 *
 * @param[in] delayTime_ms The time to block the thread in milliseconds.
 */
void delay_ms(const uint16_t& delayTime_ms) noexcept;

/**
 * @brief Block the calling thread for the given time measured in microseconds.
 *
 * @param[in] delayTime_us The time to block the thread in microseconds.
 */
void delay_us(const uint16_t& delayTime_us) noexcept;

/**
 * @brief Structure used for removing references in order to maintain value categories,
 *        for instance between function calls.
 *
 * @tparam T The value type.
 */
template<typename T>
struct RemoveReference
{
    typedef T type;
};

/**
 * @brief Maintain the value category of given object.
 *
 * @param[in] object Reference to object whose value category is to be maintained.
 *
 * @return Given object with maintained value category.
 */
template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type& object) noexcept;

/**
 * @brief Maintain the value category of given object.
 *
 * @param[in] object Reference to object whose value category is to be maintained.
 *
 * @return Given object with maintained value category.
 */
template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type&& object) noexcept;

/**
 * @brief Enable interrupts globally.
 */
void globalInterruptEnable() noexcept;

/**
 * @brief Disable interrupts globally.
 */
void globalInterruptDisable() noexcept;

/**
 * @brief Set a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to set.
 */
template<typename T = uint8_t>
constexpr void set(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Set bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to set.
 * @param[in] bits The other bits to set.
 */
template<typename T = uint8_t, typename... Bits>
constexpr void set(volatile T& reg, uint8_t bit, Bits&&... bits) noexcept;

/**
 * @brief Clear a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to clear.
 */
template<typename T = uint8_t>
constexpr void clear(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Clear bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to clear.
 * @param[in] bits The other bits to clear.
 */
template<typename T = uint8_t, typename... Bits>
constexpr void clear(volatile T& reg, uint8_t bit, Bits&&... bits) noexcept;

/**
 * @brief Toggle a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to toggle.
 */
template<typename T = uint8_t>
constexpr void toggle(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Toggle bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to toggle.
 * @param[in] bits The other bits to toggle.
 */
template<typename T = uint8_t, typename... Bits>
constexpr void toggle(volatile T& reg, uint8_t bit, Bits&&... bits) noexcept;

/**
 * @brief Read a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param[in, out] reg Reference to the register to read from.
 * @param[in] bit The given bit to read.
 *
 * @return True if the bit is set, false otherwise.
 */
template<typename T = uint8_t>
constexpr bool read(const volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Read bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param[in, out] reg Reference to the register to read from.
 * @param[in] bit The first bit to read.
 * @param[in] bits The other bits to read.
 *
 * @return True if at least one of bits is set, false otherwise.
 */
template<typename T = uint8_t, typename... Bits>
constexpr bool read(const volatile T& reg, uint8_t bit, Bits&&... bits) noexcept;

/**
 * @brief Calculate the power out of given base and exponent, where power = base ^ exponent.
 *
 * @tparam T1 The base type. Must be arithmetic.
 * @tparam T2 The exponent type. Must be arithmetic.
 * @tparam T3 The power type. Must be arithmetic.
 *
 * @param[in] base The base to use for the calculation.
 * @param[in] exponent The exponent to use for the calculation.
 *
 * @return The power as a function of given base and exponent.
 */
template<typename T1 = double, typename T2 = double, typename T3 = double>
T3 power(T1 base, T2 exponent) noexcept;

/**
 * @brief Round given number to nearest integer.
 *
 * @tparam T1 The type to round to. Must be integral.
 * @tparam T2 The type to round from. Must be arithmetic.
 *
 * @param[in] number The number to round.
 *
 * @return The corresponding rounded number.
 */
template<typename T1 = int32_t, typename T2 = double>
constexpr T1 round(T2 value) noexcept;

/**
 * @brief Check if the given number is within the given range [min, max].
 *
 * @tparam T The numeric type. Must be of arithmetic type.
 *
 * @param[in] number The number to check.
 * @param[in] min Minimum value of the range to check.
 * @param[in] max Maximum value of the range to check.
 *
 * @return True if the given number is within the given range, false otherwise.
 */
template<typename T>
constexpr bool inRange(T number, T min, T max) noexcept;

/**
 * @brief Allocate a new object on the heap.
 *
 * @tparam T The field type.
 * @tparam Args The types of arguments to pass to the constructor of T.
 *
 * @param[in] args The arguments to pass to the constructor of T.
 *
 * @return A pointer to the new object.
 *
 *         If the memory allocation fails, a nullptr is returned.
 */
template<typename T, typename... Args>
inline T* newObject(Args&&... args) noexcept;

/**
 * @brief Allocate a new field on the heap.
 *
 * @tparam T The field type.
 *
 * @param[in] size The size of field in number of elements it can hold (default = 1).
 *
 * @return A pointer to the new object.
 *
 *         If the memory allocation fails, a nullptr is returned.
 */
template<typename T>
inline T* newMemory(size_t size = 1U) noexcept;

/**
 * @brief Resize referenced heap allocated block via reallocation.
 *
 * @tparam T The block type.
 *
 * @param[in] block The block to resize.
 * @param[in] newSize The new size of allocated block, i.e. the number of elements it can hold
 *                    after reallocation.
 *
 * @return A pointer to the resized block at success, else a nullptr.
 */
template<typename T>
inline T* reallocMemory(T* block, size_t newSize) noexcept;

/**
 * @brief Delete heap allocated block via deallocation.
 *
 *        The pointer to the block is set to null after deallocation.
 *
 * @tparam T The block type.
 *
 * @param[in] block Reference to the block to delete.
 */
template<typename T>
inline void deleteMemory(T*& block) noexcept;

/**
 * @brief Move memory from given source to a copy.
 *
 * The copy will gain ownership of memory and the source instance will be emptied.
 *
 * @tparam T The type of source and the copy.
 *
 * @param[in] source Reference to the source whose memory is moved.
 *
 * @return The copy of source.
 */
template<typename T>
inline T move(T&& source) noexcept;

} // namespace utils

#include "impl/utils_impl.h"