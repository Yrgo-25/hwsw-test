/**
 * @brief Implementation details of class memory::SharedPtr and associated
 *        factory functions.
 *
 * @note Don't include this header, use <shared_ptr.h> instead!
 */
#pragma once

namespace memory
{

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::SharedPtr(T* data) noexcept
    : myData{data}
    , myRefCount{newRefCount()}
{}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) noexcept
    : myData{other.myData}
    , myRefCount{other.myRefCount}
{
    (*myRefCount)++;
}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept
    : myData{other.myData}
    , myRefCount{other.myRefCount}
{
    other.myData     = nullptr;
    other.myRefCount = nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::~SharedPtr() noexcept
{
    reset();
}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) noexcept
{
    if (this != &other)
    {
        release();
        myData     = other.myData;
        myRefCount = other.myRefCount;
        (*myRefCount)++;
    }
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept
{
    if (this != &other)
    {
        release();
        myData           = other.myData;
        myRefCount       = other.myRefCount;
        other.myData     = nullptr;
        other.myRefCount = nullptr;
    }
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::operator bool() const
{
    return myData != nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
T& SharedPtr<T>::operator*() noexcept
{
    return *myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T& SharedPtr<T>::operator*() const noexcept
{
    return *myData;
}

// -----------------------------------------------------------------------------
template<typename T>
T* SharedPtr<T>::operator->() noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T* SharedPtr<T>::operator->() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
T* SharedPtr<T>::get() noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
const T* SharedPtr<T>::get() const noexcept
{
    return myData;
}

// -----------------------------------------------------------------------------
template<typename T>
void SharedPtr<T>::reset(T* newData) noexcept
{
    release();
    myData     = newData;
    myRefCount = newRefCount();
}

// -----------------------------------------------------------------------------
template<typename T>
T* SharedPtr<T>::release() noexcept
{
    decrementReferenceCount();
    myData     = nullptr;
    myRefCount = nullptr;
    return nullptr;
}

// -----------------------------------------------------------------------------
template<typename T>
size_t* SharedPtr<T>::newRefCount() noexcept
{
    return new size_t{1U};
}

// -----------------------------------------------------------------------------
template<typename T>
void SharedPtr<T>::decrementReferenceCount() noexcept
{
    if (myRefCount && (--(*myRefCount) == 0U)) { releaseMemory(); }
}

// -----------------------------------------------------------------------------
template<typename T>
void SharedPtr<T>::releaseMemory() noexcept
{
    utils::deleteMemory(myData);
    utils::deleteMemory(myRefCount);
    myData     = nullptr;
    myRefCount = nullptr;
}

// -----------------------------------------------------------------------------
template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) noexcept
{
    return SharedPtr<T>{utils::newObject(utils::forward<Args>(args)...)};
}

// -----------------------------------------------------------------------------
template<typename T, size_t Size>
SharedPtr<T> makeShared() noexcept
{
    return SharedPtr<T>{utils::newMemory<T>(Size)};
}

} // namespace memory