/* LINKED ARRAY.cpp
 *   by Lut99
 *
 * Created:
 *   30/07/2021, 15:00:56
 * Last edited:
 *   04/08/2021, 18:27:37
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the LinkedArray class, which is our implementation of a
 *   linked list.
**/

#include <cstring>
#include <stdexcept>

#include "LinkedArray.hpp"


/***** LINKEDARRAYSTORAGE CLASS *****/
/* Default constructor for the LinkedArrayStorage, which initializes it to empty. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayStorage<T, SIZE_T>::LinkedArrayStorage() :
    head(nullptr),
    tail(nullptr),
    size(0)
{}

/* Copy constructor for the LinkedArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayStorage<T, SIZE_T>::LinkedArrayStorage(const LinkedArrayStorage& other) :
    head(nullptr),
    tail(nullptr),
    size(other.size)
{
    // If there's nothing to copy, stop
    if (other.head == nullptr) { return; }

    // Otherwise, copy at least the head
    this->head = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (this->head == nullptr) { throw std::bad_alloc(); }
    new(&this->head->value) T(other.head->value);
    this->head->next = nullptr;
    this->head->prev = nullptr;

    // Now, copy the rest of the chain
    LinkedArrayLink<T>* this_link = this->head;
    LinkedArrayLink<T>* other_link = other.head->next;
    while (other_link != nullptr) {
        // Copy the link
        this_link->next = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
        if (this->head == nullptr) { throw std::bad_alloc(); }
        new(&this_link->next->value) T(other_link->value);
        this_link->next->next = nullptr;
        this_link->next->prev = this_link;

        // Advance the links
        this_link = this_link->next;
        other_link = other_link->next;
    }

    // Make the tail point to the last in our chain
    this->tail = this_link;

    // Done
    return;
}

/* Move constructor for the LinkedArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayStorage<T, SIZE_T>::LinkedArrayStorage(LinkedArrayStorage&& other) :
    head(other.head),
    tail(other.tail),
    size(other.size)
{
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
}

/* Destructor for the LinkedArrayStorage class. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayStorage<T, SIZE_T>::~LinkedArrayStorage() {
    LinkedArrayLink<T>* link = this->head;
    while (link != nullptr) {
        // Start deleting the lot
        LinkedArrayLink<T>* next_link = link->next;
        if constexpr (std::conjunction<std::is_destructible<T>, std::negation<std::is_trivially_destructible<T>>>::value) {
            link->value.~T();
        }
        free(link);
        link = next_link;
    }
}





/***** LINKEDARRAYITERATOR CLASS *****/
/* Increments the iterator by one (prefix). */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>& Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator++() {
    // If we're not a nullptr, move to the next one
    if (this->link != nullptr) {
        this->link = this->link->next;
    }
    
    // D0ne
    return *this;
}

/* Increments the iterator by one (postfix). */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T> Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator++(int) const {
    // Copy ourselves before returning
    LinkedArrayIterator<T> result = *this;

    // If we're not a nullptr, move to the next one
    if (this->link != nullptr) {
        this->link = this->link->next;
    }
    
    // D0ne
    return result;
}

/* Decrements the iterator by one (prefix). */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>& Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator--() {
    // If we're not a nullptr, move to the previous one
    if (this->link != nullptr) {
        this->link = this->link->prev;
    }
    
    // D0ne
    return *this;
}

/* Decrements the iterator by one (postfix). */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T> Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator--(int) const {
    // Copy ourselves before returning
    LinkedArrayIterator<T> result = *this;

    // If we're not a nullptr, move to the previous one
    if (this->link != nullptr) {
        this->link = this->link->prev;
    }
    
    // D0ne
    return result;
}



/* Increments the iterator by N steps. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>& Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator+=(SIZE_T N) {
    // Loop as far as we can
    for (SIZE_T i = 0; i < N && this->link != nullptr; i++) {
        this->link = this->link->next;
    }

    // Done
    return *this;
}

/* Decrements the iterator by N steps. */
template <class T, class SIZE_T>
Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>& Makma3D::Tools::_linked_array_intern::LinkedArrayIterator<T, SIZE_T>::operator-=(SIZE_T N) {
    // Loop as far as we can
    for (SIZE_T i = 0; i < N && this->link != nullptr; i++) {
        this->link = this->link->prev;
    }

    // Done
    return *this;
}





/***** LINKEDARRAY CLASS *****/
/* Default constructor for the LinkedArray, which initializes it to an empty list. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::LinkedArray() {}

/* Constructor for the LinkedArray class, which takes a single element and repeats that the given amount of times. Makes use of the element's copy constructor. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::LinkedArray(const T& elem, SIZE_T n_repeats) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we don't allocate anything, stop
    if (n_repeats == 0) { return; }

    // Otherwise, continue by allocating a head
    this->storage.head = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (this->storage.head == nullptr) { throw std::bad_alloc(); }
    new(&this->storage.head->value) T(elem);
    this->storage.head->next = nullptr;
    this->storage.head->prev = nullptr;

    // Allocate the rest
    LinkedArrayLink<T>* link = this->storage.head;
    for (SIZE_T i = 1; i < n_repeats; i++) {
        // Create the new link
        link->next = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
        if (link->next == nullptr) { throw std::bad_alloc(); }
        new(&link->next->value) T(elem);
        link->next->next = nullptr;
        link->next->prev = link;

        // Update the link & the size
        link = link->next;
    }

    // Set the tail and the length, then d0ne
    this->storage.tail = link;
    this->storage.size = n_repeats;
    return;
}

/* Constructor for the LinkedArray class, which takes a raw C-style vector to copy elements from and its size. Note that the Array's element type must have a copy custructor defined. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::LinkedArray(const T* list, SIZE_T list_size) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we don't need to allocate anything, stop
    if (list_size == 0) { return; }

    // Otherwise, continue by allocating a head
    this->storage.head = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (this->storage.head == nullptr) { throw std::bad_alloc(); }
    new(&this->storage.head->value) T(list[0]);
    this->storage.head->next = nullptr;
    this->storage.head->prev = nullptr;

    // Allocate the rest
    LinkedArrayLink<T>* link = this->storage.head;
    for (SIZE_T i = 1; i < list_size; i++) {
        // Create the new link
        link->next = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
        if (link->next == nullptr) { throw std::bad_alloc(); }
        new(&link->next->value) T(list[i]);
        link->next->next = nullptr;
        link->next->prev = link;

        // Update the link
        link = link->next;
    }

    // Set the tail & length, then d0ne
    this->storage.tail = link;
    this->storage.size = list_size;
    return;
}

/* Constructor for the LinkedArray class, which takes an initializer_list to initialize the LinkedArray with. Makes use of the element's copy constructor. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::LinkedArray(const std::initializer_list<T>& list) :
    LinkedArray(list.begin(), static_cast<SIZE_T>(list.size()))
{}

/* Constructor for the LinkedArray class, which takes a C++-style vector. Note that the Array's element type must have a copy custructor defined. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U, typename>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::LinkedArray(const std::vector<T>& list) :
    LinkedArray(list.data(), static_cast<SIZE_T>(list.size()))
{}



/* Private helper function that returns the node at the given index, going either forward or backward (whichever is shorter). */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::_linked_array_intern::LinkedArrayLink<T>* Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::_node_at(SIZE_T index) const {
    if (index >= this->storage.size) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size));
    }

    // If we're at the ends, ez
    if (index == 0) { return this->storage.head; }
    else if (index == this->storage.size - 1) { return this->storage.tail; }
    else if (index >= this->storage.size) { throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size)); }

    // Loop from either the front or the back to arrive at the given node
    LinkedArrayLink<T>* result;
    if (index < this->storage.size / 2) {
        // Go at it from the front
        result = this->storage.head;
        for (SIZE_T i = 0; i < index; i++) {
            result = result->next;
        }
    } else {
        // Take the back
        result = this->storage.tail;
        for (SIZE_T i = this->storage.size - 1; i-- > index ;) {
            result = result->prev;
        }
    }

    // D0ne
    return result;
}

/* Private helper function that actually appends a given element to the linked array. */
template <class T, class SIZE_T, bool D, bool C, bool M>
void Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::_push_back(_linked_array_intern::LinkedArrayLink<T>* new_link) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Decide if to set or append
    if (this->storage.size == 0) {
        // We don't have a head, so set it as ours
        this->storage.head = new_link;
        this->storage.tail = new_link;
        this->storage.size = 1;
    } else {
        // Otherwise, append it to our tail
        this->storage.tail->next = new_link;
        new_link->prev = this->storage.tail;
        this->storage.tail = new_link;
        ++this->storage.size;
    }

    // Done
    return;
}


/* Private helper function that actually prepends a given element to the linked array. */
template <class T, class SIZE_T, bool D, bool C, bool M>
void Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::_push_front(_linked_array_intern::LinkedArrayLink<T>* new_link) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Decide if to set or append
    if (this->storage.size == 0) {
        // We don't have a tail, so set it as ours
        this->storage.head = new_link;
        this->storage.tail = new_link;
        this->storage.size = 1;
    } else {
        // Otherwise, prepend it to our head
        new_link->next = this->storage.head;
        this->storage.head->prev = new_link;
        this->storage.head = new_link;
        ++this->storage.size;
    }

    // Done
    return;
}



/* Adds a whole linked array worth of new elements to this linked array, copying them. Note that this requires the elements to be copy constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::operator+=(const LinkedArray& elems) -> std::enable_if_t<C, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we're empty, then simply copy the other Array
    if (this->storage.size == 0) {
        return *this = Makma3D::Tools::LinkedArray<T>(elems);
    }

    // Otherwise, get our tail and start appending
    LinkedArrayLink<T>* this_link = this->storage.tail;
    LinkedArrayLink<T>* other_link = elems.storage.head;
    while (other_link != nullptr) {
        // Copy the element over
        this_link->next = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
        if (this_link->next == nullptr) { throw std::bad_alloc(); }
        new(&this_link->next->value) T(other_link->value);
        this_link->next->next = nullptr;
        this_link->next->prev = this_link;

        // Move both pointers onward
        this_link = this_link->next;
        other_link = other_link->next;
    }

    // Update the tail & the length, then quit
    this->storage.tail = this_link;
    this->storage.size += elems.storage.size;
    return *this;
}

/* Adds a whole linked array worth of new elements to this linked array, leaving the original array in an unused state (moving it). Does not require a move constructor to be defined. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::operator+=(LinkedArray&& elems) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we're empty, then move over
    if (this->storage.size == 0) {
        return *this = Makma3D::Tools::LinkedArray<T>(std::move(elems));
    }
    // Do nothing if the other is empty
    if (elems.storage.size == 0) {
        return *this;
    }

    // Otherwise, paste their elements at the end of our tail
    this->storage.tail->next = elems.storage.head;
    this->storage.tail->next->prev = this->storage.tail;
    this->storage.tail = elems.storage.tail;

    // Update the length, done
    this->storage.size += elems.storage.size;
    return *this;
}



/* Adds a new element of type T to the start of the linked array, initializing it with its default constructor. Only needs a default constructor to be present. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_front() -> std::enable_if_t<D, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T();
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_front(new_link);

    // D0ne
    return *this;
}

/* Adds a new element of type T to the start of the linked array, copying it. Note that this requires the element to be copy constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_front(const T& elem) -> std::enable_if_t<C, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(elem);
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_front(new_link);

    // D0ne
    return *this;
}

/* Adds a new element of type T to the start of the linked array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_front(T&& elem) -> std::enable_if_t<M, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(std::move(elem));
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_front(new_link);

    // D0ne
    return *this;
}

/* Removes the first element from the array. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::pop_front() {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we have no head, ez
    if (this->storage.size == 0) { return; }

    // Otherwise, delete the head
    LinkedArrayLink<T>* second_head = this->storage.head->next;
    if constexpr (std::is_destructible<T>::value) {
        this->storage.head->value.~T();
    }
    free(this->storage.head);

    // Set the previous one as tail (if there is one)
    if (second_head != nullptr) {
        second_head->prev = nullptr;
        this->storage.head = second_head;
    } else {
        this->storage.head = nullptr;
        this->storage.tail = nullptr;
    }

    // Done
    --this->storage.size;
    return *this;
}



/* Inserts a new element at the given location, pushing all elements coming after it one index back. Since we initialise the element with its default constructor, we need that to be present for the LinkedArray's element type. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::insert(SIZE_T index) -> std::enable_if_t<D, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If the index is 0 or the end, just call push_front or push_back, respectively.
    if (index == 0) { return this->push_front(); }
    else if (index >= this->storage.size) { throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size)); }

    // Create the new link
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T();

    // Get the index we'll be replacing and it's previous neighbour (which is guaranteed to exist due to the index == 0 check)
    LinkedArrayLink<T>* link = this->_node_at(index);
    LinkedArrayLink<T>* prev_link = link->prev;

    // Insert it into the list
    prev_link->next = new_link;
    new_link->prev = prev_link;
    link->prev = new_link;
    new_link->next = link;

    // Done
    ++this->storage.size;
    return *this;
}

/* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. Requires the element to be copy constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::insert(SIZE_T index, const T& elem) -> std::enable_if_t<C, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If the index is 0 or the end, just call push_front or push_back, respectively.
    if (index == 0) { return this->push_front(elem); }
    else if (index >= this->storage.size) { throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size)); }

    // Create the new link
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(elem);

    // Get the index we'll be replacing and it's previous neighbour (which is guaranteed to exist due to the index == 0 check)
    LinkedArrayLink<T>* link = this->_node_at(index);
    LinkedArrayLink<T>* prev_link = link->prev;

    // Insert it into the list
    prev_link->next = new_link;
    new_link->prev = prev_link;
    link->prev = new_link;
    new_link->next = link;

    // Done
    ++this->storage.size;
    return *this;
}

/* Inserts the given element at the given location, pushing all elements coming after it one index back. Requires the element to be move constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If the index is 0 or the end, just call push_front or push_back, respectively.
    if (index == 0) { return this->push_front(std::move(elem)); }
    else if (index >= this->storage.size) { throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size)); }

    // Create the new link
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(std::move(elem));

    // Get the index we'll be replacing and it's previous neighbour (which is guaranteed to exist due to the index == 0 check)
    LinkedArrayLink<T>* link = this->_node_at(index);
    LinkedArrayLink<T>* prev_link = link->prev;

    // Insert it into the list
    prev_link->next = new_link;
    new_link->prev = prev_link;
    link->prev = new_link;
    new_link->next = link;

    // Done
    ++this->storage.size;
    return *this;
}



/* Adds a new element of type T to the back of the linked array, initializing it with its default constructor. Only needs a default constructor to be present. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_back() -> std::enable_if_t<D, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T();
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_back(new_link);

    // D0ne
    return *this;
}

/* Adds a new element of type T to the back of the linked array, copying it. Note that this requires the element to be copy constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_back(const T& elem) -> std::enable_if_t<C, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(elem);
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_back(new_link);

    // D0ne
    return *this;
}

/* Adds a new element of type T to the back of the linked array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::push_back(T&& elem) -> std::enable_if_t<M, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Create the new node
    LinkedArrayLink<T>* new_link = (LinkedArrayLink<T>*) malloc(sizeof(LinkedArrayLink<T>));
    if (new_link == nullptr) { throw std::bad_alloc(); }
    new(&new_link->value) T(std::move(elem));
    new_link->next = nullptr;
    new_link->prev = nullptr;

    // Append the element
    this->_push_back(new_link);

    // D0ne
    return *this;
}

/* Removes the last element from the array. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::pop_back() {
    using namespace Makma3D::Tools::_linked_array_intern;

    // If we have no tail, ez
    if (this->storage.size == 0) { return; }

    // Otherwise, delete the tail
    LinkedArrayLink<T>* second_to_tail = this->storage.tail->prev;
    if constexpr (std::is_destructible<T>::value) {
        this->storage.tail->value.~T();
    }
    free(this->storage.tail);

    // Set the previous one as tail (if there is one)
    if (second_to_tail != nullptr) {
        second_to_tail->next = nullptr;
        this->storage.tail = second_to_tail;
    } else {
        this->storage.head = nullptr;
        this->storage.tail = nullptr;
    }

    // Done
    --this->storage.size;
    return *this;
}



/* Erases an element with the given index from the linked array. Does nothing if the index is out-of-bounds. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::erase(SIZE_T index) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Make sure we have enough elements
    if (index >= this->storage.size) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range for LinkedArray of length " + std::to_string(this->storage.size));
    }

    // Loop to the target link
    LinkedArrayLink<T>* link = this->_node_at(index);

    // Get the neighbours
    LinkedArrayLink<T>* prev_link = link->prev;
    LinkedArrayLink<T>* next_link = link->next;

    // Remove the link itself
    if constexpr (std::is_destructible<T>::value) {
        link->value.~T();
    }
    free(link);

    // Link the neighbours to each other (if they exist)
    if (prev_link != nullptr) {
        prev_link->next = next_link;
    }
    if (next_link != nullptr) {
        next_link->prev = prev_link;
    }

    // D0ne, decrement the length
    --this->storage.size;
    return *this;
}

/* Erases an element referenced by the given iterator. Note that this invalidates the iterator. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::erase(const iterator& iter) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Simply get the target link directly from the iterator
    LinkedArrayLink<T>* link = iter.link;
    if (link == nullptr) { throw std::invalid_argument("Cannot delete iterator pointing beyond the LinkedArray."); }

    // Get the neighbours
    LinkedArrayLink<T>* prev_link = link->prev;
    LinkedArrayLink<T>* next_link = link->next;

    // Remove the link itself
    if constexpr (std::is_destructible<T>::value) {
        link->value.~T();
    }
    free(link);

    // Link the neighbours to each other (if they exist)
    if (prev_link != nullptr) {
        prev_link->next = next_link;
    }
    if (next_link != nullptr) {
        next_link->prev = prev_link;
    }

    // D0ne, decrement the length
    --this->storage.size;
    return *this;

}

/* Erases all elements until (and including) the element referenced by the given iterator. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::erase_until(const iterator& iter) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Get the link pointer to by the iterator
    LinkedArrayLink<T>* link = iter.link;
    if (link == nullptr) { throw std::invalid_argument("Cannot delete elements up to an iterator that is beyond the LinkedArray."); }

    // Remove it and all its preceding neighbours from the array
    this->storage.head = link->next;
    if (link->next != nullptr) { link->next->prev = nullptr; }

    // Delete all things
    while (link != nullptr) {
        // Get the 'next' link
        LinkedArrayLink<T>* prev_link = link->prev;

        // Delete the current one
        if constexpr (std::is_destructible<T>::value) {
            link->value.~T();
        }
        free(link);

        // Move to the next, noting that we deleted it
        --this->storage.size;
        link = prev_link;
    }

    // Done
    return *this;
}

/* Erases all elements from (and including) the element referenced by the given iterator. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::erase_from(const iterator& iter) {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Get the link pointer to by the iterator
    LinkedArrayLink<T>* link = iter.link;
    if (link == nullptr) { throw std::invalid_argument("Cannot delete elements from an iterator that is beyond the LinkedArray."); }

    // Remove it and all its preceding neighbours from the array
    if (link->prev != nullptr) { link->prev->next = nullptr; }
    this->storage.tail = link->prev;

    // Delete all things
    while (link != nullptr) {
        // Get the next link
        LinkedArrayLink<T>* next_link = link->next;

        // Delete the current one
        if constexpr (std::is_destructible<T>::value) {
            link->value.~T();
        }
        free(link);

        // Move to the next, noting that we deleted it
        --this->storage.size;
        link = next_link;
    }

    // Done
    return *this;
}

/* Erases everything from the linked array, thus deallocating all its elements. */
template <class T, class SIZE_T, bool D, bool C, bool M>
Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>& Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::reset() {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Simply delete everything
    LinkedArrayLink<T>* link = this->storage.head;
    while (link != nullptr) {
        // Start deleting the lot
        LinkedArrayLink<T>* next_link = link->next;
        if constexpr (std::is_destructible<T>::value) {
            link->value.~T();
        }
        free(link);
        link = next_link;
    }

    // Reset the values
    this->storage.head = nullptr;
    this->storage.tail = nullptr;
    this->storage.size = 0;

    // Return
    return *this;
}



/* Resizes the linked array to the given size. Any leftover elements will be initialized with their default constructor (and thus requires the type to have one), and elements that won't fit will be deallocated. */
template <class T, class SIZE_T, bool D, bool C, bool M>
template <typename U>
auto Makma3D::Tools::LinkedArray<T, SIZE_T, D, C, M>::resize(SIZE_T new_size) -> std::enable_if_t<D, U> {
    using namespace Makma3D::Tools::_linked_array_intern;

    // Check if we need to scale down or size up
    if (new_size < this->storage.size) {
        // Scale down; remove the N last elements
        for (SIZE_T i = 0; i < this->storage.size - new_size; i++) {
            this->pop_back();
        }
    } else if (new_size > this->storage.size) {
        // Scale up; allocate N new, default elements
        for (SIZE_T i = 0; i < new_size - this->storage.size; i++) {
            this->push_back();
        }
    }

    // Done
    return *this;
}
