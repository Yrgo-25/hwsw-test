/**
 * @brief Unique pointer implementation.
 */
#pragma once

#include "utils/utils.h"

namespace memory
{
/**
 * @brief Unique pointer implementation.
 *
 *        This class is non-copyable.
 *
 * @tparam T The pointer type.
 */
template<typename T>
class UniquePtr final
{
public:
    /**
     * @brief Create new unique pointer.
     *
     * @param[in] data Pointer to data for which to take ownership (default = none).
     */
    explicit UniquePtr(T* data = nullptr) noexcept;

    /**
     * @brief Create new unique pointer, which takes ownership over memory owned by other pointer.
     *
     * @param[in] other Reference to other unique pointer to move memory from.
     */
    UniquePtr(UniquePtr<T>&& other) noexcept;

    /**
     * @brief Release allocated resources before deletion.
     */
    ~UniquePtr() noexcept;

    /**
     * @brief Move resources from other unique pointer.
     *
     * @param[in] other Reference to other unique pointer to move memory from.
     *
     * @return Reference to this unique pointer.
     */
    UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept;

    /**
     * @brief Check if the pointer isn't null.
     *
     * @return True if the pointer isn't null, false otherwise.
     */
    operator bool() const;

    /**
     * @brief Overload of operator * to provide held data.
     *
     * @return Reference to held data.
     */
    T& operator*() noexcept;

    /**
     * @brief Overload of operator * to provide held data.
     *
     * @return Reference to held data.
     */
    const T& operator*() const noexcept;

    /**
     * @brief Overload of operator -> to provide held data.
     *
     * @return Pointer to held data.
     */
    T* operator->() noexcept;

    /**
     * @brief Overload of operator -> to provide held data.
     *
     * @return Pointer to held data.
     */
    const T* operator->() const noexcept;

    /**
     * @brief Get pointer to held data.
     *
     * @return Pointer to held data.
     */
    T* get() noexcept;

    /**
     * @brief Get pointer to held data.
     *
     * @return Pointer to held data.
     */
    const T* get() const noexcept;

    /**
     * @brief Reset unique pointer by releasing currently held data.
     *
     * @param[in] newData Pointer to new data to take ownership over (default = none).
     */
    void reset(T* newData = nullptr) noexcept;

    /**
     * @brief Release ownership over held data.
     *
     * @return Pointer to currently held data.
     */
    T* release() noexcept;

    UniquePtr(const UniquePtr<T>&)               = delete; // No copy constructor.
    UniquePtr<T>& operator=(const UniquePtr<T>&) = delete; // No copy assignment.

private:
    T* myData; // Pointer to unique data.
};

/**
 * @brief Create unique pointer taking ownership of given data.
 *
 * @tparam T The pointer type.
 * @tparam Args The types of arguments to pass to the constructor of T.
 *
 * @param[in] args The arguments to pass to the constructor of T.
 *
 * @return Unique pointer holding ownership over given data.
 */
template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) noexcept;

/**
 * @brief Create unique pointer pointing at new field of given size.
 *
 * @tparam T    The pointer/field type.
 * @tparam Size The size of new field.
 *
 * @return Unique pointer holding ownership over the new field.
 */
template<typename T, size_t Size>
UniquePtr<T> makeUnique() noexcept;

} // namespace memory

#include "impl/unique_ptr_impl.h"
