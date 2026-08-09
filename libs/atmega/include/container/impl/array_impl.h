/**
 * @brief Implementation details of container::Array class.
 *
 * @note Don't include this header, use <array.h> instead!
 */
#pragma once

namespace container
{
// -----------------------------------------------------------------------------
template<typename T, size_t Size>
Array<T, Size>::Array() noexcept
    : myData{}
{}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<typename... Values>
Array<T, Size>::Array(const Values&&... values) noexcept
    : Array()
{
    const T array[sizeof...(values)] = {(values)...};
    copy(array);
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
Array<T, Size>::Array(const T (&values)[Size]) noexcept
    : Array()
{
    copy(values);
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
Array<T, Size>::Array(const Array<T, Size>& other) noexcept
    : myData{other.myData}
{}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
Array<T, Size>::Array(Array<T, Size>&& other) noexcept
    : myData{other.myData}
{
    other.clear();
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
Array<T, Size>& Array<T, Size>::operator=(const Array<T, ValueCount>& other) noexcept
{
    copy(other);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
Array<T, Size>& Array<T, Size>::operator=(Array<T, Size>&& other) noexcept
{
    copy(other);
    other.clear();
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
Array<T, Size>& Array<T, Size>::operator=(const T (&values)[ValueCount]) noexcept
{
    copy(values);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
Array<T, Size>& Array<T, Size>::operator+=(const Array<T, ValueCount>& other) noexcept
{
    copy(other, ValueCount);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
Array<T, Size>& Array<T, Size>::operator+=(const T (&values)[ValueCount]) noexcept
{
    copy(values, ValueCount);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
T& Array<T, Size>::operator[](const size_t index) noexcept
{
    return myData[index];
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
const T& Array<T, Size>::operator[](const size_t index) const noexcept
{
    return myData[index];
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
T* Array<T, Size>::data() noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
T* Array<T, Size>::data() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
size_t Array<T, Size>::size() const noexcept
{
    return Size;
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::Iterator Array<T, Size>::begin() noexcept
{
    return Iterator{myData};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::ConstIterator Array<T, Size>::begin() const noexcept
{
    return ConstIterator{myData};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::Iterator Array<T, Size>::end() noexcept
{
    return Iterator{myData + Size};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::ConstIterator Array<T, Size>::end() const noexcept
{
    return ConstIterator{myData + Size};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::Iterator Array<T, Size>::rbegin() noexcept
{
    return Iterator{myData + Size - 1U};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::ConstIterator Array<T, Size>::rbegin() const noexcept
{
    return ConstIterator{myData + Size - 1U};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::Iterator Array<T, Size>::rend() noexcept
{
    return Iterator{myData - 1U};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
typename Array<T, Size>::ConstIterator Array<T, Size>::rend() const noexcept
{
    return ConstIterator{myData - 1U};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
void Array<T, Size>::clear() noexcept
{
    for (auto& i : *this)
    {
        i = {};
    }
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
void Array<T, Size>::copy(const T (&values)[ValueCount], const size_t offset) noexcept
{
    for (size_t i{}; i + offset < Size && i < ValueCount; ++i)
    {
        myData[offset + i] = values[i];
    }
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
template<size_t ValueCount>
void Array<T, Size>::copy(const Array<T, ValueCount>& other, const size_t offset) noexcept
{
    for (size_t i{}; i + offset < Size && i < other.Size(); ++i)
    {
        myData[offset + i] = other[i];
    }
}
} // namespace container
