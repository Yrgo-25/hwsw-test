/**
 * @brief Implementation of dynamic vectors of any type.
 */
#pragma once

#include <stddef.h>

namespace container
{
/**
 * @brief Class for implementation of dynamic vectors.
 *
 * @tparam T The vector type.
 */
template<typename T>
class Vector
{
public:
    class Iterator;      // Vector iterator.
    class ConstIterator; // Constant vector iterator.

    /**
     * @brief Create empty vector.
     */
    Vector() noexcept;

    /**
     * @brief Create vector of given size.
     *
     * @param[in] size The size of vector, i.e. the number of elements it can hold.
     */
    explicit Vector(size_t size) noexcept;

    /**
     * @brief Create vector containing given values.
     *
     * @tparam Values Parameter pack containing values.
     *
     * @param[in] values The values to add to the vector.
     */
    template<typename... Values>
    explicit Vector(const Values&&... values) noexcept;

    /**
     * @brief Create vector as a copy of another vector.
     *
     * @param[in] other Reference to other vector to copy from.
     */
    Vector(const Vector<T>& other) noexcept;

    /**
     * @brief Move memory from another vector.
     *
     *        The other vector is emptied once the move operation is completed.
     *
     * @param[in] other Reference to other vector to move memory from.
     */
    Vector(Vector&& other) noexcept;

    /**
     * @brief Delete vector.
     */
    ~Vector() noexcept;

    /**
     * @brief Copy the content of vector to assigned vector.
     *
     *        Previous values are cleared before copying.
     *
     * @param[in] other Reference to vector holding the data to copy.
     *
     * @return Reference to this vector.
     */
    Vector<T>& operator=(const Vector<T>& other) noexcept;

    /**
     * @brief Move the content from other vector.
     *
     *        Previous values are cleared before copying.
     *
     *        The other vector is emptied once the move operation is completed.
     *
     * @param[in] other Reference to vector holding the data to move.
     *
     * @return Reference to this vector.
     */
    Vector<T>& operator=(Vector<T>&& other) noexcept;

    /**
     * @brief Assign given values to vector.
     *
     *        Previous values are cleared before copying.
     *
     * @tparam Values Type of the given values.
     *
     * @param[in] values Reference to given values.
     *
     * @return Reference to this vector.
     */
    template<typename... Values>
    Vector<T>& operator=(const Values&&... values) noexcept;

    /**
     * @brief Add values from another vector.
     *
     * @param[in] other Reference to vector holding the values to add.
     *
     * @return Reference to this vector.
     */
    Vector<T>& operator+=(const Vector<T>& other) noexcept;

    /**
     * @brief Push referenced values to the back of vector.
     *
     * @tparam ValueCount The number of values to add.
     *
     * @param[in] values Reference to the values to add.
     *
     * @return Reference to this vector.
     */
    template<size_t ValueCount>
    Vector<T>& operator+=(const T (&values)[ValueCount]) noexcept;

    /**
     * @brief Get element at given index in the vector.
     *
     * @param[in] index Index of requested element.
     *
     * @return Reference to the element at given index.
     */
    T& operator[](size_t index) noexcept;

    /**
     * @brief Get element at given index in the vector.
     *
     * @param[in] index Index of requested element.
     *
     * @return Reference to the element at given index.
     */
    const T& operator[](size_t index) const noexcept;

    /**
     * @brief Get the data held by the vector.
     *
     * @return Pointer to the beginning of vector.
     */
    const T* data() const noexcept;

    /**
     * @brief Get the size of vector in the number of elements it can hold.
     *
     * @return The size of vector as an unsigned integer.
     */
    size_t size() const noexcept;

    /**
     * @brief Check if the vector is empty.
     *
     * @return True if the vector vector is empty, false otherwise.
     */
    bool empty() const noexcept;

    /**
     * @brief Get the beginning of vector.
     *
     * @return Iterator pointing at the beginning of the vector.
     */
    Iterator begin() noexcept;

    /**
     * @brief Get the beginning of vector.
     *
     * @return Iterator pointing at the beginning of the vector.
     */
    ConstIterator begin() const noexcept;

    /**
     * @brief Get the end of vector.
     *
     * @return Iterator pointing at the end of the vector.
     */
    Iterator end() noexcept;

    /**
     * @brief Get the end of vector.
     *
     * @return Iterator pointing at the end of the vector.
     */
    ConstIterator end() const noexcept;

    /**
     * @brief Get the reverse beginning of the vector.
     *
     * @return Iterator pointing at the reverse beginning of the vector.
     */
    Iterator rbegin() noexcept;

    /**
     * @brief Get the reverse beginning of the vector.
     *
     * @return Iterator pointing at the reverse beginning of the vector.
     */
    ConstIterator rbegin() const noexcept;

    /**
     * @brief Get the reverse end of the vector.
     *
     * @return Iterator pointing at the reverse end of the vector.
     */
    Iterator rend() noexcept;

    /**
     * @brief Get the reverse end of vector.
     *
     * @return Iterator pointing at the reverse end of the vector.
     */
    ConstIterator rend() const noexcept;

    /**
     * @brief Get the address of last element of vector.
     *
     * @return Pointer to the last element of vector.
     */
    T* last() noexcept;

    /**
     * @brief Get the address of last element of vector.
     *
     * @return Pointer to the last element of vector.
     */
    const T* last() const noexcept;

    /**
     * @brief Clear content of vector.
     */
    void clear() noexcept;

    /**
     * @brief Resize the vector to given new size.
     *
     * @param[in] newSize The new size of vector.
     *
     * @return True if the vector was resized, false otherwise.
     */
    bool resize(size_t newSize) noexcept;

    /**
     * @brief Push new value to the back of vector.
     *
     * @param[in] value Reference to the new value to push to the vector.
     *
     * @return True if the value was pushed to the back of vector, false otherwise.
     */
    bool pushBack(const T& value) noexcept;

    /**
     * @brief Pop value at the back of vector noexcept.
     *
     * @return True if the last value of vector was popped, false otherwise.
     */
    bool popBack() noexcept;

protected:
    bool copy(const Vector<T>& other) noexcept;
    void assign(const Vector<T>& other, size_t offset = 0) noexcept;

    template<size_t ValueCount>
    void assign(const T (&values)[ValueCount], size_t offset = 0) noexcept;

    bool addValues(const Vector<T>& other) noexcept;

    template<size_t ValueCount>
    bool addValues(const T (&values)[ValueCount]) noexcept;

    /** Pointer to dynamic field holding data. */
    T* myData;

    /** The size of the field in number of elements it can hold. */
    size_t mySize;
};
} // namespace container

#include "impl/vector_impl.h"
#include "iterator/vector_iterator.h"
