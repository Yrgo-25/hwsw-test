/**
 * @brief Implementation details of container::List class.
 *
 * @note Don't include this header, use <list.h> instead!
 */
#pragma once

#include "utils/utils.h"

namespace container
{
/**
 * @brief Implementation of node holding data in a linked list.
 *
 * @param[in] T The node type, i.e. the type of the stored data.
 */
template<typename T>
struct List<T>::Node
{
    Node* previous; // Pointer to previous node.
    Node* next;     // Pointer to next data.
    T data;         // Data the node holds.

    static Node* create(const T& data) noexcept;
    static void destroy(Node* self) noexcept;
    static Node* get(Iterator& iterator) noexcept;
    static const Node* get(ConstIterator& iterator) noexcept;
};

// -----------------------------------------------------------------------------
template<typename T>
List<T>::List() noexcept
    : myFirst{nullptr}
    , myLast{nullptr}
    , mySize{nullptr}
{}

// -----------------------------------------------------------------------------
template<typename T>
List<T>::List(const size_t size, const T& startValue) noexcept
    : List()
{
    resize(size, startValue);
}

// -----------------------------------------------------------------------------
template<typename T>
template<typename... Values>
List<T>::List(const Values&&... values) noexcept
{
    const T array[sizeof...(values)]{(values)...};
    addValues(array);
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>::~List() noexcept
{
    clear();
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>::List(const List<T>& other) noexcept
    : List()
{
    copy(other);
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>::List(List<T>&& other) noexcept
    : myFirst{other.myFirst}
    , myLast{other.myLast}
    , mySize{other.mySize}
{
    other.myFirst = nullptr;
    other.myLast  = nullptr;
    other.mySize  = 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>& List<T>::operator=(const List<T>& other) noexcept
{
    clear();
    copy(other);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>& List<T>::operator=(List<T>&& other) noexcept
{
    clear();
    myFirst = other.myFirst;
    myLast  = other.myLast;
    mySize  = other.mySize;

    other.myFirst = nullptr;
    other.myLast  = nullptr;
    other.mySize  = 0U;
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
List<T>& List<T>::operator+=(const List<T>& other) noexcept
{
    copy(other);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
List<T>& List<T>::operator+=(const T (&values)[ValueCount]) noexcept
{
    addValues(values);
    return *this;
}

// -----------------------------------------------------------------------------
template<typename T>
T& List<T>::operator[](Iterator& iterator) noexcept
{
    return *iterator;
}

// -----------------------------------------------------------------------------
template<typename T>
const T& List<T>::operator[](ConstIterator& iterator) const noexcept
{
    return *iterator;
}

// -----------------------------------------------------------------------------
template<typename T>
size_t List<T>::size() const noexcept
{
    return mySize;
}

// -----------------------------------------------------------------------------
template<typename T>
void List<T>::clear() noexcept
{
    removeAllNodes();
    myFirst = nullptr;
    myLast  = nullptr;
    mySize  = 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::empty() const noexcept
{
    return mySize == 0U;
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Iterator List<T>::begin() noexcept
{
    return mySize > 0U ? Iterator{myFirst} : Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::ConstIterator List<T>::begin() const noexcept
{
    return mySize > 0U ? ConstIterator{myFirst} : ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Iterator List<T>::end() noexcept
{
    return Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::ConstIterator List<T>::end() const noexcept
{
    return ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Iterator List<T>::rbegin() noexcept
{
    return mySize > 0U ? Iterator{myLast} : Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::ConstIterator List<T>::rbegin() const noexcept
{
    return mySize > 0U ? ConstIterator{myLast} : ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Iterator List<T>::rend() noexcept
{
    return Iterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::ConstIterator List<T>::rend() const noexcept
{
    return ConstIterator{nullptr};
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::resize(const size_t newSize, const T& startValue) noexcept
{
    while (mySize < newSize)
    {
        if (!pushBack(startValue)) { return false; }
    }
    while (mySize > newSize)
    {
        popFront();
    }
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::pushFront(const T& value) noexcept
{
    auto node1{Node::create(value)};
    if (node1 == nullptr) { return false; }

    if (mySize++ == 0U)
    {
        myFirst = node1;
        myLast  = node1;
    }
    else
    {
        auto node2{myFirst};
        node1->next     = node2;
        node2->previous = node1;
        myFirst         = node1;
    }
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::pushBack(const T& value) noexcept
{
    auto node2{Node::create(value)};
    if (node2 == nullptr) { return false; }

    if (mySize++ == 0U)
    {
        myFirst = node2;
        myLast  = node2;
    }
    else
    {
        auto node1{myLast};
        node1->next     = node2;
        node2->previous = node1;
        myLast          = node2;
    }
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::insert(Iterator& iterator, const T& value) noexcept
{
    if (iterator == nullptr) { return false; }
    auto node2{Node::create(value)};
    if (node2 == nullptr) return false;
    auto node1{Node::get(iterator)->previous};
    auto node3{node1->next};

    node1->next     = node2;
    node2->previous = node1;
    node2->next     = node3;
    node3->previous = node2;
    mySize++;
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
void List<T>::popFront() noexcept
{
    if (mySize <= 1U) { clear(); }
    else
    {
        auto node1{myFirst};
        auto node2{node1->next};
        node2->previous = nullptr;
        Node::destroy(node1);
        myFirst = node2;
        mySize--;
    }
}

// -----------------------------------------------------------------------------
template<typename T>
void List<T>::popBack() noexcept
{
    if (mySize <= 1U) { clear(); }
    else
    {
        auto node2{myLast};
        auto node1{node2->previous};
        node1->next = nullptr;
        Node::destroy(node2);
        myLast = node1;
        mySize--;
    }
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::remove(Iterator& iterator) noexcept
{
    if (iterator == nullptr) { return false; }
    else
    {
        auto node2{iterator.get()};
        auto node1{node2->previous};
        auto node3{node2->next};
        node1->next     = node3;
        node3->previous = node1;
        Node::destroy(node2);
        mySize--;
        return true;
    }
}

// -----------------------------------------------------------------------------
template<typename T>
bool List<T>::copy(const List<T>& other) noexcept
{
    for (size_t i{}; i < other.size(); ++i)
    {
        if (!pushBack(other[i])) { return false; }
    }
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
void List<T>::assign(const T (&values)[ValueCount], const size_t offset) noexcept
{
    for (size_t i{}; i < ValueCount && offset + i < mySize; ++i)
    {
        (*this)[offset + i] = values[i];
    }
}

// -----------------------------------------------------------------------------
template<typename T>
template<size_t ValueCount>
bool List<T>::addValues(const T (&values)[ValueCount]) noexcept
{
    if (ValueCount == 0U) { return false; }
    for (size_t i{}; i < ValueCount; ++i)
    {
        if (!pushBack(values[i])) { return false; }
    }
    return true;
}

// -----------------------------------------------------------------------------
template<typename T>
void List<T>::removeAllNodes() noexcept
{
    for (auto i{begin()}; i != end();)
    {
        auto next{Node::get(i)->next};
        Node::destroy(Node::get(i));
        i = next;
    }
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Node* List<T>::Node::create(const T& data) noexcept
{
    auto self{utils::newMemory<Node>()};
    if (self == nullptr) { return nullptr; }
    self->data     = data;
    self->previous = nullptr;
    self->next     = nullptr;
    return self;
}

// -----------------------------------------------------------------------------
template<typename T>
void List<T>::Node::destroy(Node* self) noexcept
{
    utils::deleteMemory(self);
}

// -----------------------------------------------------------------------------
template<typename T>
typename List<T>::Node* List<T>::Node::get(Iterator& iterator) noexcept
{
    return static_cast<Node*>(iterator.address());
}

// -----------------------------------------------------------------------------
template<typename T>
const typename List<T>::Node* List<T>::Node::get(ConstIterator& iterator) noexcept
{
    return static_cast<Node*>(iterator.address());
}
} // namespace container
