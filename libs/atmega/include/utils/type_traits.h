/**
 * @brief Implementation type traits for static checking of data types.
 */
#pragma once

#include <stdint.h>

namespace type_traits
{
/**
 * @brief Check if given type is of unsigned integral type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_unsigned
{
    // True for unsigned integral types only.
    static const bool value{false};
};

/**
 * @brief Specialization for bool as a valid unsigned integral type.
 */
template<>
struct is_unsigned<bool>
{
    static const bool value{true};
};

/**
 * @brief Specialization for uint8_t as a valid unsigned integral type.
 */
template<>
struct is_unsigned<uint8_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for uint16_t as a valid unsigned integral type.
 */
template<>
struct is_unsigned<uint16_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for uint32_t as a valid unsigned integral type.
 */
template<>
struct is_unsigned<uint32_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for uint64_t as a valid unsigned integral type.
 */
template<>
struct is_unsigned<uint64_t>
{
    static const bool value{true};
};

/**
 * @brief Check if given type is of signed integral type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_signed
{
    // True for signed integral types only.
    static const bool value{false};
};

/**
 * @brief Specialization for int8_t as a valid signed integral type.
 */
template<>
struct is_signed<int8_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for int16_t as a valid signed integral type.
 */
template<>
struct is_signed<int16_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for int32_t as a valid signed integral type.
 */
template<>
struct is_signed<int32_t>
{
    static const bool value{true};
};

/**
 * @brief Specialization for int64_t as a valid signed integral type.
 */
template<>
struct is_signed<int64_t>
{
    static const bool value{true};
};

/**
 * @brief Check if given type is of integral type, which encompasses both signed
 *        and unsigned integers.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_integral
{
    // True for integral types only.
    static const bool value{is_unsigned<T>::value || is_signed<T>::value};
};

/**
 * @brief Check if given type is of floating-point type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_floating_point
{
    // True for floating-point types only.
    static const bool value{false};
};

/**
 * @brief Specialization for float as a valid floating-point type.
 */
template<>
struct is_floating_point<float>
{
    static const bool value{true};
};

/**
 * @brief Specialization for double as a valid floating-point type.
 */
template<>
struct is_floating_point<double>
{
    static const bool value{true};
};

/**
 * @brief Check if given type is of arithmetic type, i.e. integral or floating-point type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_arithmetic
{
    // True for arithmetic types only.
    static const bool value{is_integral<T>::value || is_floating_point<T>::value};
};

/**
 * @brief Check if given type is of string type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_string
{
    // True for string types only.
    static const bool value{false};
};

/**
 * @brief Specialization for const char* as a valid string type.
 */
template<>
struct is_string<const char*>
{
    static const bool value{true};
};

/**
 * @brief Specialization for char* as a valid string type.
 */
template<>
struct is_string<char*>
{
    static const bool value{true};
};

/**
 * @brief Check if given type is a pointer.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_pointer
{
    // True for pointers only.
    static const bool value{false};
};

/**
 * @brief Specialization for all pointer types.
 *
 * @param[in] T The pointer type.
 */
template<typename T>
struct is_pointer<T*>
{
    static const bool value{true};
};

/**
 * @brief Check if given type is a reference.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_reference
{
    // True for references only.
    static const bool value{false};
};

/**
 * @brief Specialization for all reference types.
 *
 * @param[in] T The reference type.
 */
template<typename T>
struct is_reference<T&>
{
    static const bool value{true};
};
} // namespace type_traits
