/**
 * @brief Implementation details of class utils::Pair.
 *
 * @note Don't include this header, use <pair.h> instead!
 */
#pragma once

namespace utils
{
// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair() noexcept
    : first{}
    , second{}
{}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second) noexcept
    : first{first}
    , second{second}
{}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair<T1, T2>& other) noexcept
    : first{other.first}
    , second{other.second}
{}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair(Pair<T1, T2>&& other) noexcept
    : first{other.first}
    , second{other.second}
{
    other.first  = {};
    other.second = {};
}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair<T1, T2>& operator=(const Pair<T1, T2>& other) noexcept
{
    first  = other.first;
    second = other.second;
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair<T1, T2>& operator=(Pair<T1, T2>&& other) noexcept
{
    first        = other.first;
    second       = other.second;
    other.first  = {};
    other.second = {};
    return *this;
}

} // namespace utils