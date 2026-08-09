/**
 * @brief Implementation of pairs containing values of any data type.
 */
#pragma once

namespace utils
{
/**
 * @brief Class for implementation of pairs. Each pair consists of two values
 *
 *        The data type of each value can be selected arbitrary.
 *
 * @tparam T1 Type of first value of pair.
 * @tparam T2 Type of second value of pair.
 */
template<typename T1, typename T2>
struct Pair
{
    T1 first{};  // First value of pair.
    T2 second{}; // Second value of pair.

    /**
     * @brief Create empty pair.
     */
    Pair() noexcept;

    /**
     * @brief Create pair containing given values.
     *
     * @param[in] first  Reference to the first value of pair.
     * @param[in] second Reference to the second value of pair.
     */
    explicit Pair(const T1& first, const T2& second) noexcept;

    /**
     * @brief Create copy of other pair.
     *
     * @param[in] other Reference to other pair to copy from.
     */
    Pair(const Pair<T1, T2>& other) noexcept;

    /**
     * @brief Create new pair taking ownership of data held by other pair.
     *
     *        The other pair is emptied once the new pair has taken ownership over the data.
     *
     * @param[in] other Reference to other pair to move data from.
     */
    Pair(Pair<T1, T2>&& other) noexcept;

    /**
     * @brief Delete pair.
     */
    ~Pair() noexcept = default;

    /**
     * @brief Copy data from other pair.
     *
     * @param[in] other Reference to other pair to copy from.
     *
     * @return Reference to this pair.
     */
    Pair<T1, T2>& operator=(const Pair<T1, T2>& other) noexcept;

    /**
     * @brief Move data from other pair.
     *
     *        The other pair is emptied once the new pair has taken ownership over the data.
     *
     * @param[in] other Reference to other pair to move data from.
     *
     * @return Reference to this pair.
     */
    Pair<T1, T2>& operator=(Pair<T1, T2>&& other) noexcept;
};
} // namespace utils

#include "impl/pair_impl.h"