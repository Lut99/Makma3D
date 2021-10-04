/* QUEUE ARRAY.hpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 21:37:01
 * Last edited:
 *   04/10/2021, 21:37:01
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the QueueArray, which implements a linear piece of memory as
 *   a FIFO queue.
**/

#ifndef TOOLS_QUEUE_ARRAY_HPP
#define TOOLS_QUEUE_ARRAY_HPP

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <vector>

#include "ArrayTools.hpp"

namespace Makma3D::Tools {
    /* The QueueArray, which implements a very memory-efficient FIFO queue. Optimised to work for multiple threads. */
    template <class T, class SIZE_T = uint32_t, bool D = std::is_default_constructible<T>::value, bool C = std::is_copy_constructible<T>::value, bool M = std::is_move_constructible<T>::value>
    class QueueArray: public _array_intern::CopyControl<C> {
    public:
        /* The datatype stored in the array. */
        using type = T;
        /* The size type that is used in the array. */
        using size_type = SIZE_T;
    
    private:
        /* The internal data as wrapped by ArrayStorage. */
        _array_intern::ArrayStorage<T, SIZE_T> storage;
        /* The index of the 'first' element in the queue. */
        SIZE_T head;
        /* Points after the 'last' element in the queue. */
        SIZE_T tail;


        /* Private helper function that computes the number of elements left in the queue. */
        inline SIZE_T _size() const { return this->head >= this->tail ? this->head - this->tail : this->tail - this->head; }
    
    public:
        /* Default constructor for the QueueArray class, which initializes it to not having any elements. */
        QueueArray();
        /* Constructor for the QueueArray class, which takes an initial amount to set its capacity to. Each element will thus be uninitialized. */
        QueueArray(SIZE_T initial_size);
        /* Constructor for the QueueArray class, which takes a single element and repeats that the given amount of times. Makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        QueueArray(const T& elem, SIZE_T n_repeats);

        /* Creates a new queue that is a copy of this queue with the given element copied and appended to it. Note that this requires the elements to be copy constructible. */
        template <typename U = QueueArray>
        inline auto operator+(const T& elem) const -> std::enable_if_t<C, U> { return QueueArray(*this).operator+=(elem); }
        /* Creates a new queue that is a copy of this queue with the given element append to it (moving it). Note that this still requires the elements to have some form of a move constructor defined. */
        template <typename U = QueueArray>
        inline auto operator+(T&& elem) const -> std::enable_if_t<M, U> { return QueueArray(*this).operator+=(std::move(elem)); }
        /* Adds the given element at the end of this queue, copying it. Note that this requires the elements to be copy constructible. */
        template <typename U = QueueArray&>
        inline auto operator+=(const T& elem) -> std::enable_if_t<C, U> { return this->push_back(elem); }
        /* Adds the given element at the end of this queue, moving it. Note that this still requires the elements to have some form of a move constructor defined. */
        template <typename U = QueueArray&>
        inline auto operator+=(T&& elem) -> std::enable_if_t<M, U> { return this->push_back(std::move(elem)); }

        /* Addition operator for an element and a mathing QueueArray to prepend it. Requires the element to be copy constructible. */
        template <typename U = QueueArray&>
        friend inline auto operator+(const QueueArray& queue_array, const T& elem) -> std::enable_if_t<C, U> { return QueueArray(queue_array).push_front(elem); }
        /* Addition operator for an element and a mathing QueueArray to prepend it. Requires the element to be move constructible. */
        template <typename U = QueueArray&>
        friend inline auto operator+(const QueueArray& queue_array, T&& elem) -> std::enable_if_t<M, U> { return QueueArray(queue_array).push_front(std::move(elem)); }

        /* Adds a new element of type T to the back of the queue. The element is initialized with with its default constructor and thus needs a default constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
        template <typename U = Array&>
        auto push() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of the queue. The element is initialized as a copy of the given element, and thus needs a copy constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
        template <typename U = Array&>
        auto push(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of the queue. The element is moved from the given element, and thus needs a move constructor to be present. Note that this never resizes the internal array, so throws errors if the queue is full. */
        template <typename U = Array&>
        auto push(T&& elem) -> std::enable_if_t<M, U>;
        /* Adds a new element of type T to the back of the queue. The element is initialized with with its default constructor and thus needs a default constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
        template <typename U = Array&>
        auto push_wait() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of the queue. The element is initialized as a copy of the given element, and thus needs a copy constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
        template <typename U = Array&>
        auto push_wait(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of the queue. The element is moved from the given element, and thus needs a move constructor to be present. Note that this never resizes the internal array, but instead blocks the thread until space is available. */
        template <typename U = Array&>
        auto push_wait(T&& elem) -> std::enable_if_t<M, U>;

        /* Removes the first element from the queue and returns it. The element is copied (so it needs a copy constructor), which is horribly inefficient when compared to the move-based pop. However, will be useful in case the type has no move constructor. */
        template <typename U = T>
        auto pop_copy() -> std::enable_if_t<C, U>;
        /* Removes the first element from the queue and returns it. Needs a move constructor to remove the element from the internal Array safely. */
        template <typename U = T>
        auto pop() -> std::enable_if_t<M, U>;
        /* Removes the first element from the queue and returns it, blocking this thread (idly) until such element is present. The element is copied (so it needs a copy constructor), which is horribly inefficient when compared to the move-based pop. However, will be useful in case the type has no move constructor. */
        template <typename U = T>
        auto pop_copy_wait() -> std::enable_if_t<C, U>;
        /* Removes the first element from the queue and returns it, blocking this thread (idly) until such element is present. Needs a move constructor to remove the element from the internal Array safely. */
        template <typename U = T>
        auto pop_wait() -> std::enable_if_t<M, U>;

        /* Erases everything from the queue, but leaves the internally located array intact. */
        QueueArray& clear();
        /* Erases everything from the queue, even removing the internal allocated array. */
        QueueArray& reset();

        /* Re-allocates the internal array to the given size. Any leftover elements will be left unitialized, and elements that won't fit will be deallocated. */
        template <typename U = QueueArray&>
        auto hard_reserve(SIZE_T new_size) -> std::enable_if_t<M, U>;
        /* Guarantees that the QueueArray has at least min_size capacity after the call. Does so by reallocating the internal array if we currently have less, but leaving it untouched otherwise. Any new elements will be left unitialized. */
        template <typename U = QueueArray&>
        auto reserve(SIZE_T min_size) -> std::enable_if_t<M, U>;

        /* Returns a muteable reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline T& operator[](SIZE_T index) { return this->storage.elements[this->head + index]; }
        /* Returns a constant reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline const T& operator[](SIZE_T index) const { return this->storage.elements[this->head + index]; }
        /* Returns a muteable reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        T& at(SIZE_T index);
        /* Returns a constant reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        inline const T& at(SIZE_T index) const { return const_cast<QueueArray*>(this)->at(index); }
        /* Returns the first element in the queue (without popping it). */
        inline T& first() { return this->storage.elements[this->head]; }
        /* Returns the first element in the queue (without popping it). */
        inline const T& first() const { return this->storage.elements[this->head]; }
        /* Returns the last element in the queue. */
        inline T& last() { return this->storage.elements[this->tail - 1]; }
        /* Returns the last element in the queue. */
        inline const T& last() const { return this->storage.elements[this->tail - 1]; }

        /* Returns true if there are no elements in this queue, or false otherwise. */
        inline bool empty() const { return this->storage.size == 0; }
        /* Returns the number of elements stored in this queue. */
        inline SIZE_T size() const { return this->storage.size; }
        /* Returns the number of elements this queue can store before resizing. */
        inline SIZE_T capacity() const { return this->storage.capacity; }

        /* Swap operator for the QueueArray class. */
        friend void swap(QueueArray& a1, QueueArray& a2) {
            using std::swap;

            swap(a1.storage, a2.storage);
            swap(a1.head, a2.head);
            swap(a1.tail, a2.tail);
        }

    };

}

// Include the .cpp since it's all templated
#include "QueueArray.cpp"

#endif
