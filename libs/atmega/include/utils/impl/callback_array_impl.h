/**
 * @brief Implementation details of container::CallbackArray class.
 *
 * @note Don't include this header, use <callback_array.h> instead!
 */
#pragma once

namespace container
{
// -----------------------------------------------------------------------------
template<size_t Size>
CallbackArray<Size>::CallbackArray() noexcept = default;

// -----------------------------------------------------------------------------
template<size_t Size>
CallbackArray<Size>::~CallbackArray() noexcept = default;

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::add(void (*callback)(), const size_t index) noexcept
{
    if (!isIndexValid(index) || (callback == nullptr)) { return false; }
    Array<void (*)(), Size>::myData[index] = callback;
    return true;
}

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::remove(const size_t index) noexcept
{
    if (!isIndexValid(index)) { return false; }
    Array<void (*)(), Size>::myData[index] = nullptr;
    return true;
}

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::remove(void (*callback)(), const size_t index) noexcept
{
    for (auto& myCallback : *this)
    {
        if (myCallback == callback)
        {
            myCallback = nullptr;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::invoke(const size_t index) noexcept
{
    if (!isIndexValid(index) || !isCallbackDefined(index)) { return false; }
    Array<void (*)(), Size>::myData[index]();
    return true;
}

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::isIndexValid(const size_t index) const noexcept
{
    return index < Size;
}

// -----------------------------------------------------------------------------
template<size_t Size>
bool CallbackArray<Size>::isCallbackDefined(const size_t index) const noexcept
{
    return this->myData[index] != nullptr;
}

} // namespace container