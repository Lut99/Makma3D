/* ARRAY.hpp
 *   by Lut99
 *
 * Created:
 *   27/07/2021, 16:45:17
 * Last edited:
 *   07/09/2021, 17:13:05
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains a renewed iteration of the Array class, which is supposed to
 *   be a lot cleverer by disabling functions using SFINAE instead of
 *   template specializations.
**/

#ifndef TOOLS_ARRAY_HPP
#define TOOLS_ARRAY_HPP

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <limits>

#include "ArrayTools.hpp"

namespace Makma3D::Tools {
    /* The Array class, which can be used as a container for many things. It's optimized to work for containers that rarely (but still sometimes) have to resize. */
    template <class T, class SIZE_T = uint32_t, bool D = std::is_default_constructible<T>::value, bool C = std::is_copy_constructible<T>::value, bool M = std::is_move_constructible<T>::value>
    class Array: public _array_intern::CopyControl<C> {
    public:
        /* The datatype stored in the array. */
        using type = T;
        /* The size type that is used in the array. */
        using size_type = SIZE_T;

    private:
        /* The internal data as wrapped by ArrayStorage. */
        _array_intern::ArrayStorage<T, SIZE_T> storage;

    public:
        /* Default constructor for the Array class. 
         * Initialize the Array to have no elements, and no preallocated space.*/
        Array();
        /* Constructor for the Array class, which takes an initial amount to set its capacity to. Each element will thus be uninitialized.
         * @param initial_capacity The initial capacity of the internal array. */
        Array(SIZE_T initial_capacity);
        /* Constructor for the Array class, which takes a single element and repeats that the given amount of times. 
         * Requires the element to have a copy constructor.
         * @param elem The value of the element that will be copied for each of the instantiated elements.
         * @param n_repeats The number of times we should copy the given element. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        Array(const T& elem, SIZE_T n_repeats);
        /* Constructor for the Array class, which takes a raw C-style vector to copy elements from and its size. 
         * Requires the element to have a copy constructor.
         * @param list The C-style list to copy.
         * @param list_size The size of the C-style list (in elements). */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        Array(const T* list, SIZE_T list_size);
        /* Constructor for the Array class, which takes an initializer_list to initialize the Array with. 
         * Requires the element to have a copy constructor.
         * @param list The initializer list from which to copy elements. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        Array(const std::initializer_list<T>& list);
        /* Constructor for the Array class, which takes a C++-style vector. 
         * Requires the element to have a copy constructor.
         * @param list The C++ vector who's elements we copy. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        Array(const std::vector<T>& list);

        /* Creates a new array that is a copy of this array with the given element copied and appended to it. 
         * Requires the element to have a copy constructor.
         * @param elem The element to append to the copy of the Array.
         * @returns A copy of this Array with the given element appended to it. */
        template <typename U = Array>
        inline auto operator+(const T& elem) const -> std::enable_if_t<C, U> { return Array(*this).operator+=(elem); }
        /* Creates a new array that is a copy of this array with the given element append to it (moving it). 
         * Requires the element to have a copy constructor (for copying the Array) and a move constructor (for moving the element).
         * @param elem The element to append to the copy of the Array.
         * @returns A copy of this Array with the given element appended to it. */
        template <typename U = Array>
        inline auto operator+(T&& elem) const -> std::enable_if_t<C && M, U> { return Array(*this).operator+=(std::move(elem)); }
        /* Adds the given element at the end of this array, copying it. 
         * Requires the element to have a copy constructor.
         * @param elem The element to copy and add to the Array.
         * @returns A reference to this Array with the given element appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        inline auto operator+=(const T& elem) -> std::enable_if_t<C, U> { return this->push_back(elem); }
        /* Adds the given element at the end of this array, moving it. 
         * Requires the element to have a move constructor.
         * @param elem The element to add to the end of the Array.
         * @returns A reference to this Array with the given element appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        inline auto operator+=(T&& elem) -> std::enable_if_t<M, U> { return this->push_back(std::move(elem)); }

        /* Addition operator for an element and a matching Array to prepend it. The Array will be copied before addition. 
         * Requires the Array's element to have a copy constructor.
         * @param array The Array who's copy to prepend to.
         * @param elem The element who's copy should be prepended to the given Array.
         * @returns A copy of the given Array with the given element appended to it. */
        template <typename U = Array&>
        friend inline auto operator+(const Array& array, const T& elem) -> std::enable_if_t<C, U> { return Array(array).push_front(elem); }
        /* Addition operator for an element and a matching Array to prepend it. The Array will be copied before addition. 
         * Requires the Array's element to have a copy constructor (for copying the Array) and a move constructor (for moving the given element).
         * @param array The Array who's copy to prepend to.
         * @param elem The element who should be prepended to the given Array.
         * @returns A copy of the given Array with the given element appended to it. */
        template <typename U = Array&>
        friend inline auto operator+(const Array& array, T&& elem) -> std::enable_if_t<C && M, U> { return Array(array).push_front(std::move(elem)); }

        /* Creates a new array that is a copy of this array with the elements in the given array copied and appended to them. 
         * Requires the Array's elements to have a copy constructor.
         * @param elems The list of elements to copy and append to the Array.
         * @returns A copy of this Array with the given elements appended to it. */
        template <typename U = Array>
        inline auto operator+(const Array& elems) const -> std::enable_if_t<C, U> { return Array(*this).operator+=(elems); }
        /* Creates a new array that is a copy of this array with the elements in the given array appended to them (moving them). 
         * Requires the Array's elements to have a copy constructor (for copying this Array) and a move constructor (for moving the given elements).
         * @param elems The list of elements to append to the Array.
         * @returns A copy of this Array with the given elements appended to it. */
        template <typename U = Array>
        inline auto operator+(Array&& elems) const -> std::enable_if_t<C && M, U> { return Array(*this).operator+=(std::move(elems)); }
        /* Adds a whole array worth of new elements to the array, copying them. 
         * Requires the Array's elements to have a copy constructor.
         * @param elems The list of elements to copy and append to the Array.
         * @returns A reference to this Array with the given elements appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto operator+=(const Array& elems) -> std::enable_if_t<C, U>;
        /* Adds a whole array worth of new elements to the array, moving them.
         * Requires the Array's elements to have a move constructor.
         * @param elems The list of elements to append to the Array.
         * @returns A reference to this Array with the given elements appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto operator+=(Array&& elems) -> std::enable_if_t<M, U>;

        /* Adds a new element of type T to the front of the array, pushing the rest back. The element is initialized with with its default constructor. 
         * Requires the Array's elements to have a default constructor and a move constructor (for moving the other elements a place back).
         * @returns A reference to this Array with the new element prepended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_front() -> std::enable_if_t<D && M, U>;
        /* Adds a new element of type T to the front of the array, pushing the rest back. The element is initialized as a copy of the given element. 
         * Requires the Array's elements to have a copy constructor (for copying the given element) and a move constructor (for moving the other elements a place back).
         * @param elem The element to copy and to add.
         * @returns A reference to this Array with the new element prepended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_front(const T& elem) -> std::enable_if_t<C && M, U>;
        /* Adds the given element to the front of the array, pushing the rest back. 
         * Requires the Array's elements to have a move constructor.
         * @param elem The element to add.
         * @returns A reference to this Array with the new element prepended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_front(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the first element from the array, moving the rest one index to the front. 
         * Requires the Array's elements to have a move constructor.
         * @returns A reference to this Array with the first element removed from it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto pop_front() -> std::enable_if_t<M, U>;

        /* Inserts a new element at the given location, pushing all elements coming after it one index back. Since we initialise the element with its default constructor, we need that to be present for the Array's element type. Also required is a move assign operator, so the element van be moved around in the array. */
        template <typename U = Array&>
        auto insert(SIZE_T index) -> std::enable_if_t<D && M, U>;
        /* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. Requires the element to be copy constructible, and to also be move assignable to be moved around in the array. */
        template <typename U = Array&>
        auto insert(SIZE_T index, const T& elem) -> std::enable_if_t<C && M, U>;
        /* Inserts the given element at the given location, pushing all elements coming after it one index back. Requires the element to be move constructible _and_ move assignable. */
        template <typename U = Array&>
        auto insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U>;
        /* Erases an element with the given index from the array. Does nothing if the index is out-of-bounds. */
        template <typename U = Array&>
        auto erase(SIZE_T index) -> std::enable_if_t<M, U>;
        /* Erases multiple elements in the given (inclusive) range from the array. Does nothing if the any index is out-of-bounds or if the start_index is larger than the stop_index. */
        template <typename U = Array&>
        auto erase(SIZE_T start_index, SIZE_T stop_index) -> std::enable_if_t<M, U>;

        /* Adds a new element of type T to the back of array, initializing it with its default constructor. Only needs a default constructor to be present, but cannot resize itself without a move constructor. */
        template <typename U = Array&>
        auto push_back() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of array, copying it. Note that this requires the element to be copy constructible. */
        template <typename U = Array&>
        auto push_back(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
        template <typename U = Array&>
        auto push_back(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the last element from the array. */
        Array& pop_back();

        /* Erases everything from the array, but leaves the internally located array intact. */
        Array& clear();
        /* Erases everything from the array, even removing the internal allocated array. */
        Array& reset();

        /* Re-allocates the internal array to the given size. Any leftover elements will be left unitialized, and elements that won't fit will be deallocated. */
        template <typename U = Array&>
        auto hard_reserve(SIZE_T new_size) -> std::enable_if_t<M, U>;
        /* Guarantees that the Array has at least min_size capacity after the call. Does so by reallocating the internal array if we currently have less, but leaving it untouched otherwise. Any new elements will be left unitialized. */
        template <typename U = Array&>
        auto reserve(SIZE_T min_size) -> std::enable_if_t<M, U>;
        /* Resizes the array to the given size. Any leftover elements will be initialized with their default constructor (and thus requires the type to have one), and elements that won't fit will be deallocated. */
        template <typename U = Array&>
        auto hard_resize(SIZE_T new_size) -> std::enable_if_t<D && M, U>;
        /* Resizes the array to the given size. Any leftover elements will be initialized as a copy of the given element (and thus requires the type to have a copy constructor), and elements that won't fit will be deallocated. */
        template <typename U = Array&>
        auto hard_resize(const T& elem, SIZE_T new_size) -> std::enable_if_t<C && M, U>;
        /* Guarantees that the Array has at least min_size size after the call. Does so by reallocating the internal array if we currently have less, but leaving it untouched otherwise. Any leftover elements will be initialized with their default constructor (and thus requires the type to have one). */
        template <typename U = Array&>
        auto resize(SIZE_T min_size) -> std::enable_if_t<D && M, U>;
        /* Guarantees that the Array has at least min_size size after the call. Does so by reallocating the internal array if we currently have less, but leaving it untouched otherwise. Any leftover elements will be initialized as a copy of the given element (and thus requires the type to have a copy constructor). */
        template <typename U = Array&>
        auto resize(const T& elem, SIZE_T min_size) -> std::enable_if_t<C && M, U>;

        /* Returns a muteable reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline T& operator[](SIZE_T index) { return this->storage.elements[index]; }
        /* Returns a constant reference to the element at the given index. Does not perform any in-of-bounds checking. */
        inline const T& operator[](SIZE_T index) const { return this->storage.elements[index]; }
        /* Returns a muteable reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        T& at(SIZE_T index);
        /* Returns a constant reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        inline const T& at(SIZE_T index) const { return const_cast<Array*>(this)->at(index); }
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
        inline SIZE_T capacity() const { return this->storage.capacity; }

        /* Swap operator for the Array class. */
        friend void swap(Array& a1, Array& a2) {
            using std::swap;

            swap(a1.storage, a2.storage);
        }

    };

}

// Also get the .cpp
#include "Array.cpp"

#endif
