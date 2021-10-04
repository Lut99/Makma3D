/* ARRAY TOOLS.cpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 16:39:31
 * Last edited:
 *   04/10/2021, 16:39:31
 * Auto updated?
 *   Yes
 *
 * Description:
 *   File that contains a lot of basis for the various Array classes.
**/

#include <algorithm>

#include "ArrayTools.hpp"


/***** ARRAYSTORAGE CLASS *****/
/* Default constructor for the ArrayStorage class, which initializes itself to 0. */
template <class T, class SIZE_T>
Makma3D::Tools::_array_intern::ArrayStorage<T, SIZE_T>::ArrayStorage() :
    elements(nullptr),
    size(0),
    capacity(0)
{}

/* Copy constructor for the ArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_array_intern::ArrayStorage<T, SIZE_T>::ArrayStorage(const ArrayStorage& other) :
    size(other.size),
    capacity(other.capacity)
{
    // Allocate a new array for ourselves
    this->elements = (T*) malloc(this->capacity * sizeof(T));
    if (this->elements == nullptr) { throw std::bad_alloc(); }

    // Copy everything over
    if constexpr (std::is_trivially_copy_constructible<T>::value) {
        memcpy(this->elements, other.elements, this->size * sizeof(T));
    } else {
        for (SIZE_T i = 0; i < this->size; i++) {
            new(this->elements + i) T(other.elements[i]);
        }
    }
}

/* Move constructor for the ArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_array_intern::ArrayStorage<T, SIZE_T>::ArrayStorage(ArrayStorage&& other) :
    elements(other.elements),
    size(other.size),
    capacity(other.capacity)
{
    other.elements = nullptr;
    other.size = 0;
    other.capacity = 0;
}

/* Destructor for the ArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_array_intern::ArrayStorage<T, SIZE_T>::~ArrayStorage() {
    if (this->elements != nullptr) {
        if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
            for (SIZE_T i = 0; i < this->size; i++) {
                this->elements[i].~T();
            }
        }
        free(this->elements);
    }
}





/***** STACKARRAYSTORAGE CLASS *****/
/* Default constructor for the StackArrayStorage class, which initializes itself to 0. */
template <class T, size_t SIZE, class SIZE_T>
Makma3D::Tools::_array_intern::StackArrayStorage<T, SIZE, SIZE_T>::StackArrayStorage() :
    size(0)
{}

/* Copy constructor for the StackArrayStorage class. */
template <class T, size_t SIZE, class SIZE_T>
Makma3D::Tools::_array_intern::StackArrayStorage<T, SIZE, SIZE_T>::StackArrayStorage(const StackArrayStorage& other) :
    size(other.size)
{
    // Copy everything over
    if constexpr (std::is_trivially_copy_constructible<T>::value) {
        memcpy(this->elements, other.elements, this->size * sizeof(T));
    } else {
        for (SIZE_T i = 0; i < this->size; i++) {
            new(this->elements + i) T(other.elements[i]);
        }
    }
}

/* Move constructor for the StackArrayStorage class. */
template <class T, size_t SIZE, class SIZE_T>
Makma3D::Tools::_array_intern::StackArrayStorage<T, SIZE, SIZE_T>::StackArrayStorage(StackArrayStorage&& other) :
    size(other.size)
{
    // Move everything over
    if constexpr (std::is_trivially_copy_constructible<T>::value) {
        memmove(this->elements, other.elements, this->size * sizeof(T));
    } else {
        for (SIZE_T i = 0; i < this->size; i++) {
            new(this->elements + i) T(std::move(other.elements[i]));
        }
    }
}

/* Destructor for the StackArrayStorage class. */
template <class T, size_t SIZE, class SIZE_T>
Makma3D::Tools::_array_intern::StackArrayStorage<T, SIZE, SIZE_T>::~StackArrayStorage() {
    if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
        for (SIZE_T i = 0; i < this->size; i++) {
            this->elements[i].~T();
        }
    }
}
