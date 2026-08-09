/**
 * @brief Implementation of static arrays of any type.
 */
#pragma once

#include "utils/utils.h"

namespace container
{
/**
 * @brief Class for implementation of static arrays.
 *
 * @tparam T    The array type.
 * @tparam Size The array size. Must be greater than 0.
 */
template<typename T, size_t Size>
class Array
{
    // Generate a compiler error if the array size is set to 0.
    static_assert(Size > 0U, "Array size must be greater than 0!");

public:
    /** Array iterator. */
    class Iterator;

    /** Constant array iterator. */
    class ConstIterator;

    /**
     * @brief Create empty array of given size.
     */
    Array() noexcept;

    /**
     * @brief Create array containing given values.
     *
     * @tparam Values Type of the given values.
     *
     * @param[in] values Reference to given values.
     */
    template<typename... Values>
    explicit Array(const Values&&... values) noexcept;

    /**
     * @brief Create array containing given values.
     *
     * @param[in] values Reference to given values.
     */
    explicit Array(const T (&values)[Size]) noexcept;

    /**
     * @brief Create array as a copy of another array.
     *
     * @param[in] other Reference to other array to copy from.
     */
    Array(const Array<T, Size>& other) noexcept;

    /**
     * @brief Move memory from another array.
     *
     *        The other array is emptied once the move operation is completed.
     *
     * @param[in] other Reference to other array to move memory from.
     */
    Array(Array<T, Size>&& other) noexcept;

    /**
     * @brief Delete array.
     */
    ~Array() noexcept = default;

    /**
     * @brief Copy values from another array.
     *
     * @tparam ValueCount The number of values in the other array.
     *
     * @param[in] other Reference to array holding the values to copy.
     *
     * @return Reference to this array.
     */
    template<size_t ValueCount>
    Array<T, Size>& operator=(const Array<T, ValueCount>& other) noexcept;

    /**
     * @brief Move values from another array.
     *
     *        The other array is emptied once the move operation is completed.
     *
     * @param[in] other Reference to array holding the values to move.
     *
     * @return Reference to this array.
     */
    Array<T, Size>& operator=(Array<T, Size>&& other) noexcept;

    /**
     * @brief Assign given values to array.
     *
     * @tparam ValueCount The number of values to copy.
     *
     * @param[in] values Reference to the values to copy.
     *
     * @return Reference to this array.
     */
    template<size_t ValueCount>
    Array<T, Size>& operator=(const T (&values)[ValueCount]) noexcept;

    /**
     * @brief Add values from another array.
     *
     * @tparam ValueCount The number of values in the other array.
     *
     * @param[in] other Reference to array containing the values to add.
     *
     * @return Reference to this array.
     */
    template<size_t ValueCount>
    Array& operator+=(const Array<T, ValueCount>& other) noexcept;

    /**
     * @brief Add given values to array.
     *
     * @tparam ValueCount The number of values to add.
     *
     * @param[in] values Reference to the values to add.
     *
     * @return Reference to this array.
     */
    template<size_t ValueCount>
    Array& operator+=(const T (&values)[ValueCount]) noexcept;

    /**
     * @brief Get element at given index in the array.
     *
     * @param[in] index Index of requested element.
     *
     * @return Reference to the element at given index.
     */
    T& operator[](size_t index) noexcept;

    /**
     * @brief Get element at given index in the array.
     *
     * @param[in] index Index of requested element.
     *
     * @return Reference to the element at given index.
     */
    const T& operator[](size_t index) const noexcept;

    /**
     * @brief Get data held by the array.
     *
     * @return Pointer to the data held by the array.
     */
    T* data() noexcept;

    /**
     * @brief Get data held by the array.
     *
     * @return Pointer to the data held by the array.
     */
    T* data() const noexcept;

    /**
     * @brief Get the size of the array.
     *
     * @return The size of the array measured in the number of elements it can hold.
     */
    size_t size() const noexcept;

    /**
     * @brief Get the beginning of the array.
     *
     * @return Iterator pointing at the beginning of the array.
     */
    Iterator begin() noexcept;

    /**
     * @brief Get the beginning of the array.
     *
     * @return Iterator pointing at the beginning of the array.
     */
    ConstIterator begin() const noexcept;

    /**
     * @brief Get the end of the array.
     *
     * @return Iterator pointing at the end of the array.
     */
    Iterator end() noexcept;

    /**
     * @brief Get the end of the array.
     *
     * @return Iterator pointing at the end of the array.
     */
    ConstIterator end() const noexcept;

    /**
     * @brief Get the reverse beginning of the array.
     *
     * @return Iterator pointing at the reverse beginning of the array.
     */
    Iterator rbegin() noexcept;

    /**
     * @brief Get the reverse beginning of the array.
     *
     * @return Iterator pointing at the reverse beginning of the array.
     */
    ConstIterator rbegin() const noexcept;

    /**
     * @brief Get the reverse end of the array.
     *
     * @return Iterator pointing at the reverse end of the array.
     */
    Iterator rend() noexcept;

    /**
     * @brief Get the reverse end of the array.
     *
     * @return Iterator pointing at the reverse end of the array.
     */
    ConstIterator rend() const noexcept;

    /**
     * @brief Clear array content.
     */
    void clear() noexcept;

protected:
    template<size_t ValueCount>
    void copy(const T (&values)[ValueCount], size_t offset = 0U) noexcept;

    template<size_t ValueCount>
    void copy(const Array<T, ValueCount>& other, size_t offset = 0U) noexcept;

    /** Statically-sized data field. */
    T myData[Size];
};
} // namespace container

#include "impl/array_impl.h"
#include "iterator/array_iterator.h"
