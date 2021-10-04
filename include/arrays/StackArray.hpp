/* STACK ARRAY.hpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 17:17:10
 * Last edited:
 *   04/10/2021, 17:17:10
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the StackArray class, which is a fixed-size array that lives
 *   entirely on the stack instead of partly on the heap (and is thus
 *   cheaper to allocate/deallocate). Will also be more cache-friendly in
 *   the case when it's entirely heap-allocated.
**/

#ifndef TOOLS_STACK_ARRAY_HPP
#define TOOLS_STACK_ARRAY_HPP

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <limits>

#include "ArrayTools.hpp"

namespace Makma3D::Tools {
    /* The StackArray class, which can be used as a container for many things. Due to its fixed size, it can be allocated on the Stack instead of the heap, making it faster in alocation/deallocation and more cache-friendly. */
    template <class T, size_t SIZE, class SIZE_T = uint32_t, bool D = std::is_default_constructible<T>::value, bool C = std::is_copy_constructible<T>::value, bool M = std::is_move_constructible<T>::value>
    class StackArray: public _array_intern::CopyMoveControl<C, M> {
    public:
        /* The datatype stored in the array. */
        using type = T;
        /* The size type that is used in the array. */
        using size_type = SIZE_T;

    private:
        /* The internal data as wrapped by StackArrayStorage. */
        _array_intern::StackArrayStorage<T, SIZE, SIZE_T> storage;

    public:
        /* Constructor for the StackArray class, which initializes it to empty/default. */
        StackArray();
        /* Constructor for the StackArray class, which takes a single element and repeats that the given amount of times. Cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        StackArray(const T& elem, SIZE_T n_repeats);
        /* Constructor for the StackArray class, which takes a raw C-style vector to copy elements from and its size. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        StackArray(const T* list, SIZE_T list_size);
        /* Constructor for the StackArray class, which takes an initializer_list to initialize the Array with. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        StackArray(const std::initializer_list<T>& list);
        /* Constructor for the StackArray class, which takes a C++-style vector. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        StackArray(const std::vector<T>& list);

        /* Creates a new array that is a copy of this array with the given element copied and appended to it. Will throw errors if this causes the new array to grow out of its bounds. Note that this requires the elements to be copy constructible. */
        template <typename U = StackArray>
        inline auto operator+(const T& elem) const -> std::enable_if_t<C, U> { return StackArray(*this).operator+=(elem); }
        /* Creates a new array that is a copy of this array with the given element append to it (moving it).Will throw errors if this causes the new array to grow out of its bounds.  Note that this still requires the elements to have some form of a move constructor defined. */
        template <typename U = StackArray>
        inline auto operator+(T&& elem) const -> std::enable_if_t<M, U> { return StackArray(*this).operator+=(std::move(elem)); }
        /* Adds the given element at the end of this array, copying it. Will throw errors if this causes the new array to grow out of its bounds. Note that this requires the elements to be copy constructible. */
        template <typename U = StackArray&>
        inline auto operator+=(const T& elem) -> std::enable_if_t<C, U> { return this->push_back(elem); }
        /* Adds the given element at the end of this array, moving it. Will throw errors if this causes the new array to grow out of its bounds. Note that this still requires the elements to have some form of a move constructor defined. */
        template <typename U = StackArray&>
        inline auto operator+=(T&& elem) -> std::enable_if_t<M, U> { return this->push_back(std::move(elem)); }

        /* Addition operator for an element and a mathing StackArray to prepend it. Requires the element to be copy constructible. */
        template <typename U = StackArray&>
        friend inline auto operator+(const StackArray& stack_array, const T& elem) -> std::enable_if_t<C, U> { return StackArray(stack_array).push_front(elem); }
        /* Addition operator for an element and a mathing StackArray to prepend it. Requires the element to be move constructible. */
        template <typename U = StackArray&>
        friend inline auto operator+(const StackArray& stack_array, T&& elem) -> std::enable_if_t<M, U> { return StackArray(stack_array).push_front(std::move(elem)); }

        /* Adds a new element of type T to the front of the array, pushing the rest back. The element is initialized with with its default constructor. Needs a default constructor to be present, but also to be move assignable in some way to be moved around in the array. */
        template <typename U = StackArray&>
        auto push_front() -> std::enable_if_t<D && M, U>;
        /* Adds a new element of type T to the front of the array, pushing the rest back. The element is copied using its copy constructor, which it is required to have. Also required to be move assignable to be moved around. */
        template <typename U = StackArray&>
        auto push_front(const T& elem) -> std::enable_if_t<C && M, U>;
        /* Adds a new element of type T to the front of the array, pushing the rest back. The element is left in an usused state (moving it). Note that this requires the element to be move constructible. Also required to be move assignable to be moved around. */
        template <typename U = StackArray&>
        auto push_front(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the first element from the array, moving the rest one index to the front. Needs to be move assignable to do the moving. */
        template <typename U = StackArray&>
        auto pop_front() -> std::enable_if_t<M, U>;

        /* Inserts a new element at the given location, pushing all elements coming after it one index back. Since we initialise the element with its default constructor, we need that to be present for the StackArray's element type. Also required is a move assign operator, so the element van be moved around in the array. */
        template <typename U = StackArray&>
        auto insert(SIZE_T index) -> std::enable_if_t<D && M, U>;
        /* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. Requires the element to be copy constructible, and to also be move assignable to be moved around in the array. */
        template <typename U = StackArray&>
        auto insert(SIZE_T index, const T& elem) -> std::enable_if_t<C && M, U>;
        /* Inserts the given element at the given location, pushing all elements coming after it one index back. Requires the element to be move constructible _and_ move assignable. */
        template <typename U = StackArray&>
        auto insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U>;
        /* Erases an element with the given index from the array. Does nothing if the index is out-of-bounds. */
        template <typename U = StackArray&>
        auto erase(SIZE_T index) -> std::enable_if_t<M, U>;
        /* Erases multiple elements in the given (inclusive) range from the array. Does nothing if the any index is out-of-bounds or if the start_index is larger than the stop_index. */
        template <typename U = StackArray&>
        auto erase(SIZE_T start_index, SIZE_T stop_index) -> std::enable_if_t<M, U>;

        /* Adds a new element of type T to the back of array, initializing it with its default constructor. Only needs a default constructor to be present, but cannot resize itself without a move constructor. */
        template <typename U = StackArray&>
        auto push_back() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of array, copying it. Note that this requires the element to be copy constructible. */
        template <typename U = StackArray&>
        auto push_back(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
        template <typename U = StackArray&>
        auto push_back(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the last element from the array. */
        StackArray& pop_back();

        /* Erases everything from the array, but leaves the internally located array intact. */
        StackArray& clear();

        /* Makes sure new_size elements in the StackArray are initialized. Any new elements will be initialized with their default constructor (and thus requires the type to have one), and elements that won't fit will be deallocated. */
        template <typename U = StackArray&>
        auto hard_resize(SIZE_T new_size) -> std::enable_if_t<D && M, U>;
        /* Makes sure at least new_size elements in the StackArray are initialized. Any new elements will be initialized with their default constructor (and thus requires the type to have one). */
        template <typename U = StackArray&>
        auto resize(SIZE_T min_size) -> std::enable_if_t<D && M, U>;

        /* Returns a muteable reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline T& operator[](SIZE_T index) { return this->storage.elements[index]; }
        /* Returns a constant reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline const T& operator[](SIZE_T index) const { return this->storage.elements[index]; }
        /* Returns a muteable reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        T& at(SIZE_T index);
        /* Returns a constant reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        inline const T& at(SIZE_T index) const { return const_cast<StackArray*>(this)->at(index); }
        /* Returns the first element in the list. */
        inline T& first() { return this->storage.elements[0]; }
        /* Returns the first element in the list. */
        inline const T& first() const { return this->storage.elements[0]; }
        /* Returns the last element in the list. */
        inline T& last() { return this->storage.elements[this->storage.size - 1]; }
        /* Returns the last element in the list. */
        inline const T& last() const { return this->storage.elements[this->storage.size - 1]; }

        /* Returns a muteable pointer to the internal data struct. Use this to fill the array using C-libraries, but beware that the array needs to have enough space reserved. Also note that object put here will still be deallocated by the Array using ~T(). The optional new_size parameter is used to update the size() value of the array, so it knows what is initialized and what is not. Leave it at numeric_limits<array_size_t>::max() to leave the array size unchanged. */
        T* wdata(SIZE_T new_size = std::numeric_limits<SIZE_T>::max());
        /* Returns a constant pointer to the internal data struct. Use this to read from the array using C-libraries, but beware that the array needs to have enough space reserved. */
        inline const T* rdata() const { return this->storage.elements; }
        /* Returns true if there are no elements in this array, or false otherwise. */
        inline bool empty() const { return this->storage.size == 0; }
        /* Returns the number of elements stored in this Array. */
        inline SIZE_T size() const { return this->storage.size; }
        /* Returns the number of elements this Array can store before resizing. */
        inline constexpr SIZE_T capacity() const { return this->storage.capacity; }

        /* Swap operator for the StackArray class. */
        friend void swap(StackArray& sa1, StackArray& sa2) {
            using std::swap;

            swap(sa1.storage, sa2.storage);
        }

    };

}

// Include the .cpp since it's all templated
#include "StackArray.cpp"

#endif
