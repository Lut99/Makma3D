/* QUEUE ARRAY.cpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 21:37:03
 * Last edited:
 *   04/10/2021, 21:37:03
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the QueueArray, which implements a linear piece of memory as
 *   a FIFO queue.
**/

#include <cstring>
#include <stdexcept>
#include <algorithm>

#include "QueueArray.hpp"


/***** QUEUEARRAY CLASS *****/
/* Default constructor for the QueueArray class, which initializes it to not having any elements. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::QueueArray() {}

/* Constructor for the QueueArray class, which takes an initial amount to set its capacity to. Each element will thus be uninitialized. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::QueueArray(SIZE_T initial_size) :
    Array()
{
    this->storage.capacity = initial_size;
    this->storage.elements = (T*) malloc(this->storage.capacity * sizeof(T));
    if (this->storage.elements == nullptr) { throw std::bad_alloc(); }
}

/* Constructor for the QueueArray class, which takes a single element and repeats that the given amount of times. Makes use of the element's copy constructor. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::QueueArray(const T& elem, SIZE_T n_repeats) :
    Array(n_repeats)
{
    // Make enough copies
    for (SIZE_T i = 0; i < n_repeats; i++) {
        new(this->storage.elements + this->storage.size++) T(elem);
    }
}



/* Adds a new element of type T to the back of the queue. The element is initialized with with its default constructor and thus needs a default constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push() -> std::enable_if_t<D, U> {
    // Check if there is enough space in the queue
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to already full QueueArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element at the tail location
    new(this->storage.elements + this->tail) T();

    // Update the tail and the size
    this->tail = (this->tail + 1) % this->storage.capacity;
    ++this->storage.size;
    return *this;
}

/* Adds a new element of type T to the back of the queue. The element is initialized as a copy of the given element, and thus needs a copy constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push(const T& elem) -> std::enable_if_t<C, U> {
    // Check if there is enough space in the queue
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to already full QueueArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element at the tail location
    new(this->storage.elements + this->tail) T(elem);

    // Update the tail and the size
    this->tail = (this->tail + 1) % this->storage.capacity;
    ++this->storage.size;
    return *this;
}

/* Adds a new element of type T to the back of the queue. The element is moved from the given element, and thus needs a move constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push(T&& elem) -> std::enable_if_t<M, U> {
    // Check if there is enough space in the queue
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to already full QueueArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element at the tail location
    new(this->storage.elements + this->tail) T(std::move(elem));

    // Update the tail and the size
    this->tail = (this->tail + 1) % this->storage.capacity;
    ++this->storage.size;
    return *this;
}

/* Adds a new element of type T to the back of the queue. The element is initialized with with its default constructor and thus needs a default constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push_wait() -> std::enable_if_t<D, U> {
    /* TBD */
}

/* Adds a new element of type T to the back of the queue. The element is initialized as a copy of the given element, and thus needs a copy constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push_wait(const T& elem) -> std::enable_if_t<C, U> {
    /* TBD */
}

/* Adds a new element of type T to the back of the queue. The element is moved from the given element, and thus needs a move constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::push_wait(T&& elem) -> std::enable_if_t<M, U> {
    /* TBD */
}



/* Removes the first element from the queue and returns it. The element is copied (so it needs a copy constructor), which is horribly inefficient when compared to the move-based pop. However, will be useful in case the type has no move constructor. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::pop_copy() -> std::enable_if_t<C, U> {
    // Check if there is at least one element
    if (this->storage.size == 0) {
        throw std::runtime_error("Cannot pop from empty QueueArray.");
    }

    // Copy the element at the head of the array, then deallocate it (if needed)
    T result(this->storage.elements[this->head]);
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        this->storage.elements[this->head].~T();
    }

    // Update the head, the storage and return
    this->head = (this->head + 1) % this->storage.capacity;
    --this->storage.size;
    return result;
}

/* Removes the first element from the queue and returns it. Needs a move constructor to remove the element from the internal Array safely. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::pop() -> std::enable_if_t<M, U> {
    // Check if there is at least one element
    if (this->storage.size == 0) {
        throw std::runtime_error("Cannot pop from empty QueueArray.");
    }

    // Move the element at the head of the array
    T result(std::move(this->storage.elements[this->head]));

    // Update the head, the storage and return
    this->head = (this->head + 1) % this->storage.capacity;
    --this->storage.size;
    return result;
}

/* Removes the first element from the queue and returns it, blocking this thread (idly) until such element is present. The element is copied (so it needs a copy constructor), which is horribly inefficient when compared to the move-based pop. However, will be useful in case the type has no move constructor. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::pop_copy_wait() -> std::enable_if_t<C, U> {
    /* TBD */
}

/* Removes the first element from the queue and returns it, blocking this thread (idly) until such element is present. Needs a move constructor to remove the element from the internal Array safely. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::pop_wait() -> std::enable_if_t<M, U> {
    /* TBD */    
}



/* Erases everything from the queue, but leaves the internally located array intact. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>& Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::clear() {
    // Deallocate all elements in the queue if needed
    

    // Simply set everything to 0.
    this->storage.size = 0;
}

/* Erases everything from the queue, even removing the internal allocated array. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>& Makma3D::Tools::QueueArray<T, SIZE_T, D, C, M>::reset() {

}
