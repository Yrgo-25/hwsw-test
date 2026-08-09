/**
 * @brief Implementation of list iterators.
 *
 * @note This file is included in <list.h> and shall not be included directly.
 */
#pragma once

namespace container
{
/**
 * @brief Implementation of mutable list iterators.
 *
 * @tparam T The list type.
 */
template<typename T>
class List<T>::Iterator final
{
public:
    /**
     * @brief Create empty iterator.
     */
    Iterator()
        : myNode{nullptr}
    {}

    /**
     * @brief Create iterator pointing at given node.
     *
     * @param[in] node Reference to node the iterator is to point at.
     */
    Iterator(Node& node) noexcept
        : myNode{&node}
    {}

    /**
     * @brief Create iterator pointing at given node.
     *
     * @param[in] node Pointer to node the iterator is to point at.
     */
    Iterator(Node* node) noexcept
        : myNode{node}
    {}

    /**
     * @brief Delete iterator.
     */
    ~Iterator() noexcept = default;

    /**
     * @brief Create copy of another iterator.
     *
     * @param[in] other Reference to iterator to copy.
     */
    Iterator(const Iterator& other) noexcept
        : myNode{other.myNode}
    {}

    /**
     * @brief Create iterator overtaking resources from another iterator.
     *
     *        The other iterator is set to null after the move operation is completed.
     *
     * @param[in] other Reference to iterator to move resources from.
     */
    Iterator(Iterator&& other) noexcept
        : myNode{other.myNode}
    {
        other.myNode = nullptr;
    }

    /**
     * @brief Copy another iterator.
     *
     * @param[in] other Reference to iterator to copy.
     *
     * @return Reference to this iterator.
     */
    Iterator& operator=(const Iterator& other) noexcept
    {
        if (this != &other) { myNode = other.myNode; }
        return *this;
    }

    /**
     * @brief Move resources from another iterator.
     *
     *        The other iterator is set to null after the move operation is completed.
     *
     * @param[in] other Reference to iterator to move resources from.
     *
     * @return Reference to this iterator.
     */
    Iterator& operator=(Iterator&& other) noexcept
    {
        if (this != &other)
        {
            myNode       = other.myNode;
            other.myNode = nullptr;
        }
        return *this;
    }

    /**
     * @brief Set iterator to point at next node (prefix operator).
     *
     * @return Reference to this iterator.
     */
    Iterator& operator++() noexcept
    {
        myNode = myNode->next;
        return *this;
    }

    /**
     * @brief Set iterator to point at previous node (prefix operator).
     *
     * @return Reference to this iterator.
     */
    Iterator& operator--() noexcept
    {
        myNode = myNode->previous;
        return *this;
    }

    /**
     * @brief Set iterator to point at next node (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    Iterator operator++(int) noexcept
    {
        auto previous{*this};
        myNode = myNode->next;
        return previous;
    }

    /**
     * @brief Set iterator to point at previous node (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    Iterator operator--(int) noexcept
    {
        auto previous{*this};
        myNode = myNode->previous;
        return previous;
    }

    /**
     * @brief Increment the iterator given number of times.
     *
     * @param[in] incrementCount The number of times the iterator will be incremented.
     */
    void operator+=(const size_t incrementCount) noexcept
    {
        for (size_t i{}; i < incrementCount; ++i)
        {
            myNode = myNode->next;
        }
    }

    /**
     * @brief Decrement the iterator given number of times.
     *
     * @param[in] decrementCount The number of times the iterator will be decremented.
     */
    void operator-=(const size_t decrementCount) noexcept
    {
        for (size_t i{}; i < decrementCount; ++i)
        {
            myNode = myNode->previous;
        }
    }

    /**
     * @brief Check if the iterator and referenced other iterator point at the same node.
     *
     * @param[in] other Reference to other iterator.
     *
     * @return True if the iterators point at the same node, false otherwise.
     */
    bool operator==(const Iterator& other) const noexcept { return myNode == other.myNode; }

    /**
     * @brief Check if the iterator and referenced other iterator point at different nodes.
     *
     * @param[in] other Reference to other iterator.
     *
     * @return True if the iterators point at different nodes, false otherwise.
     */
    bool operator!=(const Iterator& other) const noexcept { return myNode != other.myNode; }

    /**
     * @brief Get the value held by the node the iterator is pointing at.
     *
     * @return Reference to the value held by the node the iterator is pointing at.
     */
    T& operator*() noexcept { return myNode->data; }

    /**
     * @brief Get the value held by the node the iterator is pointing at.
     *
     * @return Reference to the value held by the node the iterator is pointing at.
     */
    const T& operator*() const noexcept { return myNode->data; }

    /**
     * @brief Get the address of the node the iterator is pointing at.
     *
     * @note A void pointer is returned to keep information about nodes
     *       private within the List<T> class.
     *
     * @return Pointer to the node the iterator is pointing at.
     */
    void* address() noexcept { return myNode; }

private:
    Node* myNode; // Pointer to the node this iterator is referring to.
};

/**
 * @brief Implementation of constant list iterators.
 *
 * @tparam T The list type.
 */
template<typename T>
class List<T>::ConstIterator
{
public:
    /**
     * @brief Create empty iterator.
     */
    ConstIterator()
        : myNode{nullptr}
    {}

    /**
     * @brief Create iterator pointing at given node.
     *
     * @param[in] node Reference to node the iterator is to point at.
     */
    ConstIterator(const Node& node) noexcept
        : myNode{&node}
    {}

    /**
     * @brief Create iterator pointing at given node.
     *
     * @param[in] node Pointer to node the iterator is to point at.
     */
    ConstIterator(const Node* node) noexcept
        : myNode{node}
    {}

    /**
     * @brief Delete iterator.
     */
    ~ConstIterator() noexcept = default;

    /**
     * @brief Create copy of another iterator.
     *
     * @param[in] other Reference to iterator to copy.
     */
    ConstIterator(const ConstIterator& other) noexcept
        : myNode{other.myNode}
    {}

    /**
     * @brief Create iterator overtaking resources from another iterator.
     *
     *        The other iterator is set to null after the move operation is completed.
     *
     * @param[in] other Reference to iterator to move resources from.
     */
    ConstIterator(ConstIterator&& other) noexcept
        : myNode{other.myNode}
    {
        other.myNode = nullptr;
    }

    /**
     * @brief Copy another iterator.
     *
     * @param[in] other Reference to iterator to copy.
     *
     * @return Reference to this iterator.
     */
    ConstIterator& operator=(const ConstIterator& other) noexcept
    {
        if (this != &other) { myNode = other.myNode; }
        return *this;
    }

    /**
     * @brief Move resources from another iterator.
     *
     *        The other iterator is set to null after the move operation is completed.
     *
     * @param[in] other Reference to iterator to move resources from.
     *
     * @return Reference to this iterator.
     */
    ConstIterator& operator=(ConstIterator&& other) noexcept
    {
        if (this != &other)
        {
            myNode       = other.myNode;
            other.myNode = nullptr;
        }
        return *this;
    }

    /**
     * @brief Set iterator to point at next node (prefix operator).
     *
     * @return Reference to this iterator.
     */
    ConstIterator& operator++() noexcept
    {
        myNode = myNode->next;
        return *this;
    }

    /**
     * @brief Set iterator to point at previous node (prefix operator).
     *
     * @return Reference to this iterator.
     */
    ConstIterator& operator--() noexcept
    {
        myNode = myNode->previous;
        return *this;
    }

    /**
     * @brief Set iterator to point at next node (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    ConstIterator operator++(int) noexcept
    {
        auto previous{*this};
        myNode = myNode->next;
        return previous;
    }

    /**
     * @brief Set iterator to point at previous node (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    ConstIterator operator--(int) noexcept
    {
        auto previous{*this};
        myNode = myNode->previous;
        return previous;
    }

    /**
     * @brief Increment the iterator given number of times.
     *
     * @param[in] incrementCount The number of times the iterator will be incremented.
     */
    void operator+=(const size_t incrementCount) noexcept
    {
        for (size_t i{}; i < incrementCount; ++i)
        {
            myNode = myNode->next;
        }
    }

    /**
     * @brief Decrement the iterator given number of times.
     *
     * @param[in] decrementCount The number of times the iterator will be decremented.
     */
    void operator-=(const size_t decrementCount) noexcept
    {
        for (size_t i{}; i < decrementCount; ++i)
        {
            myNode = myNode->previous;
        }
    }

    /**
     * @brief Check if the iterator and referenced other iterator point at the same node.
     *
     * @param[in] other Reference to other iterator.
     *
     * @return True if the iterators point at the same node, false otherwise.
     */
    bool operator==(const ConstIterator& other) const noexcept { return myNode == other.myNode; }

    /**
     * @brief Check if the iterator and referenced other iterator point at different nodes.
     *
     * @param[in] other Reference to other iterator.
     *
     * @return True if the iterators point at different nodes, false otherwise.
     */
    bool operator!=(const ConstIterator& other) const noexcept { return myNode != other.myNode; }

    /**
     * @brief Get the value held by the node the iterator is pointing at.
     *
     * @return Reference to the value held by the node the iterator is pointing at.
     */
    const T& operator*() const noexcept { return myNode->data; }

    /**
     * @brief Get the address of the node the iterator is pointing at.
     *
     * @note A void pointer is returned to keep information about nodes
     *       private within the List<T> class.
     *
     * @return Pointer to the node the iterator is pointing at.
     */
    const void* address() const noexcept { return myNode; }

private:
    /** Pointer to the node this iterator is referring to. */
    const Node* myNode;
};
} // namespace container
