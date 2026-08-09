/**
 * @brief Implementation details of class memory::UniquePtr and associated
 *        factory functions.
 *
 * @note Don't include this header, use <unique_ptr.h> instead!
 */
#pragma once

namespace memory
{

// -----------------------------------------------------------------------------
template<typename T>
UniquePtr<T>::UniquePtr(T* data) noexcept
    : myData{data}
{}

// -----------------------------------------------------------------------------
template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& other) noexcept
    : myData{other.myData}
{
    other.myData = nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
UniquePtr<T>::~UniquePtr() noexcept
{
    reset();
}

// -----------------------------------------------------------------------------
template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) noexcept
{
    if (this != &other)
    {
        reset(other.myData);
        other.myData = nullptr;
    }
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
UniquePtr<T>::operator bool() const
{
    return myData != nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
T& UniquePtr<T>::operator*() noexcept
{
    return *myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T& UniquePtr<T>::operator*() const noexcept
{
    return *myData;
}

// -----------------------------------------------------------------------------
template<typename T>
T* UniquePtr<T>::operator->() noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T* UniquePtr<T>::operator->() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
T* UniquePtr<T>::get() noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T* UniquePtr<T>::get() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
void UniquePtr<T>::reset(T* newData) noexcept
{
    utils::deleteMemory(myData);
    myData = newData;
}

// -----------------------------------------------------------------------------
template<typename T>
T* UniquePtr<T>::release() noexcept
{
    T* copy{myData};
    myData = nullptr;
    return copy;
}

// -----------------------------------------------------------------------------
template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) noexcept
{
    return UniquePtr<T>{utils::newObject(utils::forward<Args>(args)...)};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
UniquePtr<T> makeUnique()
{
    return UniquePtr<T>{utils::newMemory<T>(Size)};
}

} // namespace memory