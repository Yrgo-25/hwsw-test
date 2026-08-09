/**
 * @brief Shared pointer implementation.
 */
#pragma once

#include "utils/utils.h"

namespace memory
{
/**
 * @brief Shared pointer implementation.
 *
 * @tparam T The pointer type.
 */
template<typename T>
class SharedPtr final
{
public:
    /**
     * @brief Create new shared pointer.
     *
     * @param[in] data Pointer to data for which to take ownership (default = none).
     */
    SharedPtr(T* data = nullptr) noexcept;

    /**
     * @brief Create new shared pointer, which shares ownership with another pointer.
     *
     * @param[in] other Reference to other shared pointer to copy from.
     */
    SharedPtr(const SharedPtr<T>& other) noexcept;

    /**
     * @brief Create new shared pointer, which takes ownership over memory owned by other pointer.
     *
     * @param[in] other Reference to other shared pointer to move memory from.
     */
    SharedPtr(SharedPtr<T>&& other) noexcept;

    /**
     * @brief Release allocated resources before deletion.
     *
     * @note Deletion only occurs if this is the last pointer to point at the shared memory.
     */
    ~SharedPtr() noexcept;

    /**
     * @brief Copy resources from other shared pointer.
     *
     * @param[in] other Reference to other shared pointer to copy from.
     *
     * @return Reference to this shared pointer.
     */
    SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept;

    /**
     * @brief Move resources from other shared pointer.
     *
     * @param[in] other Reference to other shared pointer to move memory from.
     *
     * @return Reference to this shared pointer.
     */
    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;

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
     * @brief Reset shared pointer by releasing currently held data.
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

private:
    static size_t* newRefCount() noexcept;
    void decrementReferenceCount() noexcept;
    void releaseMemory() noexcept;

    T* myData;          // Pointer to shared data/memory.
    size_t* myRefCount; // The number of pointers sharing ownership of data.
};

/**
 * @brief Create shared pointer taking ownership of given data.
 *
 * @tparam T The pointer type.
 * @tparam Args The types of arguments to pass to the constructor of T.
 *
 * @param[in] args The arguments to pass to the constructor of T.
 *
 * @return Shared pointer holding ownership over given data.
 */
template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) noexcept;

/**
 * @brief Create shared pointer pointing at new field of given size.
 *
 * @tparam T    The pointer/field type.
 * @tparam Size The size of new field.
 *
 * @return Shared pointer holding ownership over the new field.
 */
template<typename T, size_t Size>
SharedPtr<T> makeShared() noexcept;

} // namespace memory

#include "impl/shared_ptr_impl.h"
