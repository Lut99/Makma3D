/* STACK ARRAY.cpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 17:16:59
 * Last edited:
 *   04/10/2021, 17:16:59
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the StackArray class, which is a fixed-size array that lives
 *   entirely on the stack instead of partly on the heap (and is thus
 *   cheaper to allocate/deallocate). Will also be more cache-friendly in
 *   the case when it's entirely heap-allocated.
**/

#include <stdexcept>
#include <algorithm>

#include "StackArray.hpp"


/***** STACKARRAY CLASS *****/
/* Constructor for the StackArray class, which initializes it to empty/default. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::StackArray() {}

/* Constructor for the StackArray class, which takes a single element and repeats that the given amount of times. Cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::StackArray(const T& elem, SIZE_T n_repeats) {
    // Make enough copies
    for (SIZE_T i = 0; i < n_repeats; i++) {
        new(this->storage.elements + this->storage.size++) T(elem);
    }
}

/* Constructor for the StackArray class, which takes a raw C-style vector to copy elements from and its size. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::StackArray(const T* list, SIZE_T list_size) {
    // Copy all the elements over
    if constexpr (std::is_trivially_copy_constructible<T>::value) {
        this->storage.size = list_size;
        memcpy(this->storage.elements, list, this->storage.size * sizeof(T));
    } else {
        for (SIZE_T i = 0; i < list_size; i++) {
            new(this->storage.elements + this->storage.size++) T(list[i]);
        }
    }
}

/* Constructor for the StackArray class, which takes an initializer_list to initialize the Array with. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::StackArray(const std::initializer_list<T>& list) :
    StackArray(list.begin(), static_cast<SIZE_T>(list.size()))
{}

/* Constructor for the StackArray class, which takes a C++-style vector. The array cannot be larger than the StackArray's size, and makes use of the element's copy constructor. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::StackArray(const std::vector<T>& list) :
    StackArray(list.data(), static_cast<SIZE_T>(list.size()))
{}



/* Adds a new element of type T to the front of the array, pushing the rest back. The element is initialized with with its default constructor. Needs a default constructor to be present, but also to be move assignable in some way to be moved around in the array. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_front() -> std::enable_if_t<D && M, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to front of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + 1, this->storage.elements, this->storage.size * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements) T();
    ++this->storage.size;
    return *this;
}

/* Adds a new element of type T to the front of the array, pushing the rest back. The element is copied using its copy constructor, which it is required to have. Also required to be move assignable to be moved around. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_front(const T& elem) -> std::enable_if_t<C && M, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to front of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + 1, this->storage.elements, this->storage.size * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements) T(elem);
    ++this->storage.size;
    return *this;
}

/* Adds a new element of type T to the front of the array, pushing the rest back. The element is left in an usused state (moving it). Note that this requires the element to be move constructible. Also required to be move assignable to be moved around. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_front(T&& elem) -> std::enable_if_t<M, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to front of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + 1, this->storage.elements, this->storage.size * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements) T(std::move(elem));
    ++this->storage.size;
    return *this;
}

/* Removes the first element from the array, moving the rest one index to the front. Needs to be move assignable to do the moving. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::pop_front() -> std::enable_if_t<M, U> {
    // Check if there are any elements
    if (this->storage.size == 0) { return; }

    // Delete the first element if we need to
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        this->storage.elements[0].~T();
    }

    // If there are other elements, move them forward
    if (this->storage.size > 1) {
        if constexpr (std::is_trivially_move_constructible<T>::value) {
            memmove(this->storage.elements, this->storage.elements + 1, this->storage.size * sizeof(T));
        } else {
            for (uint32_t i = 0; i < this->storage.size - 1; i++) {
                new(this->storage.elements + i) T(std::move(this->storage.elements[i + 1]));
            }
        }
    }

    // Decrement the length
    --this->storage.size;
    return *this;
}



/* Inserts a new element at the given location, pushing all elements coming after it one index back. Since we initialise the element with its default constructor, we need that to be present for the StackArray's element type. Also required is a move assign operator, so the element van be moved around in the array. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::insert(SIZE_T index) -> std::enable_if_t<D && M, U> {
    // Check if the index is within bounds
    if (index >= this->storage.size) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out-of-bounds for StackArray with size " + std::to_string(this->storage.size) + ".");
    }

    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot insert element in already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements following the index one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + index + 1, this->storage.elements + index, (this->storage.size - index - 1) * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > index + 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements + index) T();
    ++this->storage.size;
    return *this;
}

/* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. Requires the element to be copy constructible, and to also be move assignable to be moved around in the array. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::insert(SIZE_T index, const T& elem) -> std::enable_if_t<C && M, U> {
    // Check if the index is within bounds
    if (index >= this->storage.size) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out-of-bounds for StackArray with size " + std::to_string(this->storage.size) + ".");
    }

    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot insert element in already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements following the index one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + index + 1, this->storage.elements + index, (this->storage.size - index - 1) * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > index + 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements + index) T(elem);
    ++this->storage.size;
    return *this;
}

/* Inserts the given element at the given location, pushing all elements coming after it one index back. Requires the element to be move constructible _and_ move assignable. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U> {
    // Check if the index is within bounds
    if (index >= this->storage.size) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out-of-bounds for StackArray with size " + std::to_string(this->storage.size) + ".");
    }

    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot insert element in already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Move all elements following the index one place back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + index + 1, this->storage.elements + index, (this->storage.size - index - 1) * sizeof(T));
    } else {
        for (uint32_t i = this->storage.size; i-- > index + 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i - 1]));
        }
    }

    // Insert the new element, also increasing our own size
    new(this->storage.elements + index) T(std::move(elem));
    ++this->storage.size;
    return *this;
}

/* Erases an element with the given index from the array. Does nothing if the index is out-of-bounds. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::erase(SIZE_T index) -> std::enable_if_t<M, U> {
    // Check if the index is within bounds
    if (index >= this->storage.size) { return; }

    // Otherwise, delete the element if needed
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        this->storage.elements[index].~T();
    }

    // Move the other elements one back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + index, this->storage.elements + index + 1, (this->storage.size - index - 1) * sizeof(T));
    } else {
        for (SIZE_T i = index; i < this->storage.size - 1; i++) {
            new(this->storage.elements + i) T(std::move(this->storage.elements[i + 1]));
        }
    }

    // Decrease the length
    --this->storage.size;
    return *this;
}

/* Erases multiple elements in the given (inclusive) range from the array. Does nothing if the any index is out-of-bounds or if the start_index is larger than the stop_index. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::erase(SIZE_T start_index, SIZE_T stop_index) -> std::enable_if_t<M, U> {
    // Check if in bounds
    if (start_index >= this->storage.size || stop_index >= this->storage.size || start_index > stop_index) { return; }

    // Otherwise, delete the elements if needed
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        for (SIZE_T i = start_index; i <= stop_index; i++) {
            this->storage.elements[i].~T();
        }
    }

    // Move all elements following it back
    if constexpr (std::is_trivially_move_constructible<T>::value) {
        memmove(this->storage.elements + start_index, this->storage.elements + stop_index + 1, (this->storage.size - stop_index - 1) * sizeof(T));
    } else {
        for (SIZE_T i = stop_index + 1; i < this->storage.size; i++) {
            new(this->storage.elements + (i - (stop_index - start_index) - 1)) T(std::move(this->storage.elements[i]));
        }
    }

    // Decrease the length
    this->storage.size -= 1 + stop_index - start_index;
    return *this;
}



/* Adds a new element of type T to the back of array, initializing it with its default constructor. Only needs a default constructor to be present, but cannot resize itself without a move constructor. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_back() -> std::enable_if_t<D, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to back of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element to the end of the array
    new(this->storage.elements + this->storage.size++) T();
    return *this;
}

/* Adds a new element of type T to the back of array, copying it. Note that this requires the element to be copy constructible. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_back(const T& elem) -> std::enable_if_t<C, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to back of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element to the end of the array
    new(this->storage.elements + this->storage.size++) T(elem);
    return *this;
}

/* Adds a new element of type T to the back of array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::push_back(T&& elem) -> std::enable_if_t<M, U> {
    // Make sure the array has enough size
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot push element to back of already full StackArray of size " + std::to_string(this->storage.capacity) + ".");
    }

    // Add the element to the end of the array
    new(this->storage.elements + this->storage.size++) T(std::move(elem));
    return *this;
}

/* Removes the last element from the array. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>& Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::pop_back() {
    // Check if there are any elements
    if (this->storage.size == 0) { return; }

    // Delete the last element if we need to
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        this->storage.elements[this->storage.size - 1].~T();
    }

    // Decrement the length
    --this->storage.size;
    return *this;
}



/* Erases everything from the array, but leaves the internally located array intact. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>& Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::clear() {
    // Delete everything in the Array if the type wants it to
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        for (SIZE_T i = 0; i < this->storage.size; i++) {
            this->storage.elements[i].~T();
        }
    }

    // Set the new length
    this->storage.size = 0;
    return *this;
}



/* Makes sure new_size elements in the StackArray are initialized. Any new elements will be initialized with their default constructor (and thus requires the type to have one), and elements that won't fit will be deallocated. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::hard_resize(SIZE_T new_size) -> std::enable_if_t<D && M, U> {
    // Make sure there's enough space
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot (hard) resize StackArray of size " + std::to_string(this->storage.capacity) + " to size " + std::to_string(new_size) + ".");
    }

    // Determine whether to add or remove
    if (this->storage.size < new_size) {
        // Add
        for (SIZE_T i = this->storage.size; i < new_size; i++) {
            new(this->storage.elements + this->storage.size++) T();
        }
    } else if (this->storage.size > new_size) {
        // Remove
        if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
            for (SIZE_T i = new_size; i < this->storage.size; i++) {
                this->storage.elements[i].~T();
            }
        }
        this->storage.size = new_size;
    }

    // Done
    return *this;
}

/* Makes sure at least new_size elements in the StackArray are initialized. Any new elements will be initialized with their default constructor (and thus requires the type to have one). */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::resize(SIZE_T min_size) -> std::enable_if_t<D && M, U> {
    // Make sure there's enough space
    if (this->storage.size >= this->storage.capacity) {
        throw std::runtime_error("Cannot resize StackArray of size " + std::to_string(this->storage.capacity) + " to size " + std::to_string(min_size) + ".");
    }

    // Determine whether to add or remove
    if (this->storage.size < min_size) {
        // Add
        for (SIZE_T i = this->storage.size; i < min_size; i++) {
            new(this->storage.elements + this->storage.size++) T();
        }
    }

    // Done
    return *this;
}



/* Returns a muteable reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
T& Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::at(SIZE_T index) {
    if (index >= this->storage.size) { throw std::out_of_range("Index " + std::to_string(index) + " is out-of-bounds for StackArray with size " + std::to_string(this->storage.size) + "."); }
    return this->storage.elements[index];
}



/* Returns a muteable pointer to the internal data struct. Use this to fill the array using C-libraries, but beware that the array needs to have enough space reserved. Also note that object put here will still be deallocated by the Array using ~T(). The optional new_size parameter is used to update the size() value of the array, so it knows what is initialized and what is not. Leave it at numeric_limits<array_size_t>::max() to leave the array size unchanged. */
template <class T, size_t SIZE, class SIZE_T, bool D, bool C, bool M>
T* Makma3D::Tools::StackArray<T, SIZE, SIZE_T, D, C, M>::wdata(SIZE_T new_size) {
    // Update the size if it's not the max
    if (new_size != std::numeric_limits<SIZE_T>::max()) { this->storage.size = new_size; }
    // Return the pointer
    return this->storage.elements;
}
