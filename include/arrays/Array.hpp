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

        /* Inserts a new element at the given location, pushing all elements coming after it one index back. The element is initialized with with its default constructor. 
         * Requires the Array's elements to have a default constructor and a move constructor (for moving the other elements around).
         * @param index The index where to insert the new element. To be precise, this will be the index of the new element; the element already there plus all following ones will be pushed back.
         * @returns A reference to this Array with the new element inserted into it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto insert(SIZE_T index) -> std::enable_if_t<D && M, U>;
        /* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. 
         * Requires the Array's elements to have a copy constructor (for copying the given element) and a move constructor (for moving the other elements around).
         * @param index The index where to insert the new element. To be precise, this will be the index of the new element; the element already there plus all following ones will be pushed back.
         * @param elem The element who's copy will be inserted into the array.
         * @returns A reference to this Array with the new element inserted into it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto insert(SIZE_T index, const T& elem) -> std::enable_if_t<C && M, U>;
        /* Inserts the given element at the given location, pushing all elements coming after it one index back. 
         * Requires the Array's elements to have a move constructor.
         * @param index The index where to insert the new element. To be precise, this will be the index of the new element; the element already there plus all following ones will be pushed back.
         * @param elem The element which will be inserted into the array.
         * @returns A reference to this Array with the new element inserted into it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U>;
        /* Erases an element with the given index from the array. 
         * Does nothing if the index is out-of-bounds.
         * Requires the Array's elements to have a move constructor, for moving all the elements after the erased one one position back.
         * @param index The index of the element that should be removed from the Array.
         * @returns A reference to this Array with the element removed from it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto erase(SIZE_T index) -> std::enable_if_t<M, U>;
        /* Erases multiple elements in the given (inclusive) range from the array. 
         * Does nothing if the any index is out-of-bounds or if the start_index is larger than the stop_index. 
         * Requires the Array's elements to have a move constructor, for moving all the elements after the erased ones one position back.
         * @param start_index The index of the first element that should be removed from the Array.
         * @param stop_index The index of the last element that should be removed from the Array. Makes the range inclusive on both ends.
         * @returns A reference to this Array with the elements removed from it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto erase(SIZE_T start_index, SIZE_T stop_index) -> std::enable_if_t<M, U>;

        /* Adds a new element of type T to the back of array, initializing it with its default constructor. 
         * Requires the Array's elements to have a default constructor. If the element also has a move constructor, then the function resizes itself if more capacity is necessary.
         * @returns A reference to this Array with the new element appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_back() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of array, copying it. 
         * Requires the Array's elements to have a copy constructor. If the element also has a move constructor, then the function resizes itself if more capacity is necessary.
         * @returns A reference to this Array with the new element appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_back(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of array, moving it. 
         * Requires the Array's elements to have a move constructor.
         * @returns A reference to this Array with the new element appended to it. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto push_back(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the last element from the array.
         * @returns A reference to this Array with the last element removed from it. Useful for calling multiple non-returning functions in succession. */
        Array& pop_back();

        /* Erases everything from the array, but leaves the internally located array intact.
         * @returns A reference to this Array, which is completely wiped, save for its internal capacity. Useful for calling multiple non-returning functions in succession. */
        Array& clear();
        /* Erases everything from the array, even removing the internal allocated array.
         * @returns A reference to this Array, which is completely wiped. Useful for calling multiple non-returning functions in succession. */
        Array& reset();

        /* Re-allocates the internal array to the given size. 
         * Any leftover elements will be left unitialized, and elements that won't fit will be deallocated. 
         * Requires the Array's elements to have a move constructor, for moving the elements to a newly allocated array.
         * @param new_capacity The new capacity of the array (in number of elements).
         * @returns A reference to this Array with the new capacity. Useful for calling multiple non-returning functions in succession.  */
        template <typename U = Array&>
        auto hard_reserve(SIZE_T new_capacity) -> std::enable_if_t<M, U>;
        /* Guarantees that the Array has at least min_capacity capacity after the call. 
         * The array will only be resized if it currently has less size; otherwise, it will be left untouched (unlike hard_reserve). 
         * Requires the Array's elements to have a move constructor, for moving the elements to a newly allocated array.
         * @param min_capacity The new capacity of the array (in number of elements).
         * @returns A reference to this Array with the new capacity. Useful for calling multiple non-returning functions in succession.  */
        template <typename U = Array&>
        auto reserve(SIZE_T min_capacity) -> std::enable_if_t<M, U>;
        /* Resizes the array to the given size. 
         * Any leftover elements will be initialized with their default constructor, and elements that won't fit will be deallocated. 
         * Requires the Array's elements to have a default constructor and a move constructor (for moving the elements to a newly allocated array). 
         * @param new_size The new size of the array (in number of elements). 
         * @returns A reference to this Array with the new size. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto hard_resize(SIZE_T new_size) -> std::enable_if_t<D && M, U>;
        /* Resizes the array to the given size. 
         * Any leftover elements will be initialized as a copy of the given element, and elements that won't fit will be deallocated. 
         * Requires the Array's elements to have a copy constructor and a move constructor (for moving the elements to a newly allocated array).
         * @param elem The element to copy in case we need to initialize new elements. 
         * @param new_size The new size of the array (in number of elements). 
         * @returns A reference to this Array with the new size. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto hard_resize(const T& elem, SIZE_T new_size) -> std::enable_if_t<C && M, U>;
        /* Guarantees that the Array has at least min_size size after the call. 
         * Any leftover elements will be initialized with their default constructor, but the array will be left untouched if it already has that much elements initialized. 
         * Requires the Array's elements to have a default constructor and a move constructor (for moving the elements to a newly allocated array). 
         * @param new_size The new size of the array (in number of elements). 
         * @returns A reference to this Array with the new size. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto resize(SIZE_T min_size) -> std::enable_if_t<D && M, U>;
        /* Guarantees that the Array has at least min_size size after the call. 
         * Any leftover elements will be initialized as a copy of the given element, but the array will be left untouched if it already has that much elements initialized. 
         * Requires the Array's elements to have a copy constructor and a move constructor (for moving the elements to a newly allocated array). 
         * @param elem The element to copy in case we need to initialize new elements.
         * @param new_size The new size of the array (in number of elements).
         * @returns A reference to this Array with the new size. Useful for calling multiple non-returning functions in succession. */
        template <typename U = Array&>
        auto resize(const T& elem, SIZE_T min_size) -> std::enable_if_t<C && M, U>;

        /* Returns a muteable reference to the element at the given index. 
         * Does not perform any in-of-bounds checking.
         * @param index The index of the element to return. 
         * @returns A muteable reference to the requested element. */
        inline T& operator[](SIZE_T index) { return this->storage.elements[index]; }
        /* Returns an immuteable reference to the element at the given index. 
         * Does not perform any in-of-bounds checking.
         * @param index The index of the element to return. 
         * @returns An immuteable reference to the requested element. */
        inline const T& operator[](SIZE_T index) const { return this->storage.elements[index]; }
        /* Returns a muteable reference to the element at the given index. 
         * Throws errors if the given index is out-of-range.
         * @param index The index of the element to return. 
         * @returns A muteable reference to the requested element. */
        T& at(SIZE_T index);
        /* Returns an immuteable reference to the element at the given index. 
         * Throws errors if the given index is out-of-range.
         * @param index The index of the element to return. 
         * @returns An immuteable reference to the requested element. */
        inline const T& at(SIZE_T index) const { return const_cast<Array*>(this)->at(index); }
        /* Returns the first element in the list. 
         * Will do undefined behaviour if the list is empty.
         * @returns A muteable reference to the first element. */
        inline T& first() { return this->storage.elements[0]; }
        /* Returns the first element in the list. 
         * Will do undefined behaviour if the list is empty.
         * @returns An immuteable reference to the first element. */
        inline const T& first() const { return this->storage.elements[0]; }
        /* Returns the last element in the list. 
         * Will do undefined behaviour if the list is empty.
         * @returns A muteable reference to the last element. */
        inline T& last() { return this->storage.elements[this->storage.size - 1]; }
        /* Returns the last element in the list. 
         * Will do undefined behaviour if the list is empty.
         * @returns An immuteable reference to the last element. */
        inline const T& last() const { return this->storage.elements[this->storage.size - 1]; }

        /* Returns a muteable pointer to the internal data struct. 
         * Use this to fill the array using C-libraries, but beware that the array needs to have enough space reserved. 
         * Note that elements put here will still be deallocated by the Array using ~T().
         * @param new_size If anything else than the maximum value for that integer type, sets the internal size counter to that number. Prevents the need to use resize() (and thus having to allocate elements).
         * @returns A muteable pointer to the internal data struct. */
        T* wdata(SIZE_T new_size = std::numeric_limits<SIZE_T>::max());
        /* Returns an immuteable pointer to the internal data struct. 
         * Use this to read from the array using C-libraries, but beware that the array needs to have enough space reserved.
         * @returns An immuteable pointer to the internal data struct. */
        inline const T* rdata() const { return this->storage.elements; }
        /* Checks if the Array is empty or not.
         * @returns 'true' if the Array is empty (or more precisely, if its size is 0) or 'false' otherwise. */
        inline bool empty() const { return this->storage.size == 0; }
        /* Returns the size of the Array.
         * @returns The number of elements stored in this Array. */
        inline SIZE_T size() const { return this->storage.size; }
        /* Returns the capacity of the Array.
         * @returns The number of elements the Array can store before needing to reserve new space. */
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
