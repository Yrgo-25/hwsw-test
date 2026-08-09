/**
 * @brief Implementation details of container::Vector class.
 *
 * @note Don't include this header, use <vector.h> instead!
 */
#pragma once

#include "utils/utils.h"

namespace container
{
// -----------------------------------------------------------------------------
template<typename T>
Vector<T>::Vector() noexcept
    : myData{nullptr}
    , mySize{}
{}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>::Vector(const size_t size) noexcept
    : Vector()
{
    resize(size);
}

// -----------------------------------------------------------------------------
template<typename T>
template<typename... Values>
Vector<T>::Vector(const Values&&... values) noexcept
    : Vector()
{
    const T array[sizeof...(values)]{(values)...};
    addValues(array);
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>::Vector(const Vector<T>& other) noexcept
    : Vector()
{
    copy(other);
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : Vector()
{
    myData       = other.myData;
    mySize       = other.mySize;
    other.myData = nullptr;
    other.mySize = 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>::~Vector() noexcept
{
    clear();
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) noexcept
{
    clear();
    copy(other);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
    clear();
    myData       = other.myData;
    mySize       = other.mySize;
    other.myData = nullptr;
    other.mySize = 0U;
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
template<typename... Values>
Vector<T>& Vector<T>::operator=(const Values&&... values) noexcept
{
    clear();
    copy(values...);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& other) noexcept
{
    addValues(other);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
Vector<T>& Vector<T>::operator+=(const T (&values)[ValueCount]) noexcept
{
    addValues(values);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
T& Vector<T>::operator[](const size_t index) noexcept
{
    return myData[index];
}

// -----------------------------------------------------------------------------
template<typename T>
const T& Vector<T>::operator[](const size_t index) const noexcept
{
    return myData[index];
}

// -----------------------------------------------------------------------------
template<typename T>
const T* Vector<T>::data() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
size_t Vector<T>::size() const noexcept
{
    return mySize;
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::empty() const noexcept
{
    return mySize == 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::Iterator Vector<T>::begin() noexcept
{
    return Iterator{myData};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const noexcept
{
    return ConstIterator{myData};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::Iterator Vector<T>::end() noexcept
{
    return Iterator{myData + mySize};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const noexcept
{
    return ConstIterator{myData + mySize};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::Iterator Vector<T>::rbegin() noexcept
{
    return mySize > 0U ? Iterator{myData + mySize - 1U} : Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::rbegin() const noexcept
{
    return mySize > 0U ? ConstIterator{myData + mySize - 1U} : ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::Iterator Vector<T>::rend() noexcept
{
    return mySize > 0U ? Iterator{myData - 1U} : Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::rend() const noexcept
{
    return mySize > 0U ? ConstIterator{myData - 1U} : ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
T* Vector<T>::last() noexcept
{
    return mySize > 0U ? end() - 1U : nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
const T* Vector<T>::last() const noexcept
{
    return mySize > 0U ? end() - 1U : nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
void Vector<T>::clear() noexcept
{
    utils::deleteMemory<T>(myData);
    myData = nullptr;
    mySize = 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::resize(const size_t newSize) noexcept
{
    auto copy{utils::reallocMemory<T>(myData, newSize)};
    if (copy == nullptr) { return false; }
    myData = copy;
    mySize = newSize;
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::pushBack(const T& value) noexcept
{
    if (!resize(mySize + 1U)) { return false; }
    myData[mySize - 1U] = value;
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::popBack() noexcept
{
    if (mySize <= 1U)
    {
        clear();
        return true;
    }
    else { return resize(mySize - 1U); }
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::copy(const Vector<T>& other) noexcept
{
    if (!resize(other.mySize)) { return false; }
    assign(other);
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
void Vector<T>::assign(const Vector<T>& other, const size_t offset) noexcept
{
    for (size_t i{}; offset + i < mySize && i < other.mySize; ++i)
    {
        myData[offset + i] = other.myData[i];
    }
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
void Vector<T>::assign(const T (&values)[ValueCount], const size_t offset) noexcept
{
    for (size_t i{}; i < ValueCount && offset + i < mySize; ++i)
    {
        myData[offset + i] = values[i];
    }
}

// -----------------------------------------------------------------------------
template<typename T>
bool Vector<T>::addValues(const Vector<T>& other) noexcept
{
    const auto offset{mySize};
    if (!resize(mySize + other.mySize)) { return false; }
    assign(other, offset);
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
bool Vector<T>::addValues(const T (&values)[ValueCount]) noexcept
{
    const auto offset{mySize};
    if (!resize(mySize + ValueCount)) { return false; }
    assign(values, offset);
    return true;
}
} // namespace container
