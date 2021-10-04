/* LINKED ARRAY.hpp
 *   by Lut99
 *
 * Created:
 *   30/07/2021, 14:52:35
 * Last edited:
 *   07/08/2021, 22:20:32
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the LinkedArray class, which is our implementation of a
 *   linked list.
**/

#ifndef TOOLS_LINKED_ARRAY_HPP
#define TOOLS_LINKED_ARRAY_HPP

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <vector>

#include "ArrayTools.hpp"

namespace Makma3D::Tools {
    /* The LinkedArray class, which implements a LinkedList. */
    template <class T, class SIZE_T = uint32_t, bool D = std::is_default_constructible<T>::value, bool C = std::is_copy_constructible<T>::value, bool M = std::is_move_constructible<T>::value>
    class LinkedArray;

    /* Namespace used for internal stuff to the LinkedArray class. */
    namespace _linked_array_intern {
        /* A single link in the LinkedArray. */
        template <class T>
        struct LinkedArrayLink {
            /* The value stored at this location in the LinkedArray. */
            T value;
            /* Pointer to the next link in the LinkedArray. */
            LinkedArrayLink* next;
            /* Pointer to the previous link in the LinkedArray. */
            LinkedArrayLink* prev;
        };

        /* The LinkedArrayStorage class, which basically implements the rule of 7 for the LinkedArray. */
        template <class T, class SIZE_T>
        class LinkedArrayStorage {
        private:
            /* The first node in the array. */
            LinkedArrayLink<T>* head;
            /* The last node in the array. */
            LinkedArrayLink<T>* tail;
            /* The number of elements in the array. */
            SIZE_T size;
        
            /* Declare the LinkedArray as friend. */
            friend class LinkedArray<T>;

        public:
            /* Default constructor for the LinkedArrayStorage, which initializes it to empty. */
            LinkedArrayStorage();
            /* Copy constructor for the LinkedArrayStorage class. */
            LinkedArrayStorage(const LinkedArrayStorage& other);
            /* Move constructor for the LinkedArrayStorage class. */
            LinkedArrayStorage(LinkedArrayStorage&& other);
            /* Destructor for the LinkedArrayStorage class. */
            ~LinkedArrayStorage();

            /* Copy assignment operator for the LinkedArrayStorage class. */
            inline LinkedArrayStorage& operator=(const LinkedArrayStorage& other) { return *this = LinkedArrayStorage(other); }
            /* Move assignment operator for the LinkedArrayStorage class. */
            inline LinkedArrayStorage& operator=(LinkedArrayStorage&& other) { if (this != &other) { swap(*this, other); } return *this; }
            /* Swap operator for the LinkedArrayStorage class. */
            friend void swap(LinkedArrayStorage& las1, LinkedArrayStorage& las2) {
                using std::swap;

                swap(las1.head, las2.head);
                swap(las1.tail, las2.tail);
                swap(las1.size, las2.size);
            }

        };



        /* Iterator for the LinkedArray class. */
        template <class T, class SIZE_T>
        class LinkedArrayIterator {
        protected:
            /* The node where we current are. */
            LinkedArrayLink<T>* link;
        
            /* Declare the LinkedArray class as friend. */
            friend class LinkedArray<T>;

        public:
            /* Constructor for the LinkedArrayIterator class, which takes the linked array where it should start (or nullptr to indicate end()). */
            LinkedArrayIterator(LinkedArrayLink<T>* link): link(link) {}

            /* Allows the iterator to be compared with the given one. */
            inline bool operator==(const LinkedArrayIterator& other) const { return this->link == other.link; }
            /* Allows the iterator to be compared with the given one, by inequality. */
            inline bool operator!=(const LinkedArrayIterator& other) const { return !(this->link == other.link); }

            /* Increments the iterator by one (prefix). */
            LinkedArrayIterator& operator++();
            /* Increments the iterator by one (postfix). */
            LinkedArrayIterator operator++(int) const;
            /* Decrements the iterator by one (prefix). */
            LinkedArrayIterator& operator--();
            /* Decrements the iterator by one (postfix). */
            LinkedArrayIterator operator--(int) const;

            /* Increments the iterator by N steps. */
            LinkedArrayIterator& operator+=(SIZE_T N);
            /* Returns a new iterator that is N steps ahead of this one. */
            inline LinkedArrayIterator operator+(SIZE_T N) const { return LinkedArrayIterator(*this) += N; }
            /* Decrements the iterator by N steps. */
            LinkedArrayIterator& operator-=(SIZE_T N);
            /* Returns a new iterator that is N steps behind this one. */
            inline LinkedArrayIterator operator-(SIZE_T N) const { return LinkedArrayIterator(*this) -= N; }

            /* Dereferences the iterator. */
            inline T& operator*() { return link->value; }
            /* Dereferences the iterator immutably. */
            inline const T& operator*() const { return link->value; }

        };

        /* Constant iterator for the LinkedArray class, which simply inherits from the normal iterator except that it has no mutable dereferencer. */
        template <class T>
        class LinkedArrayConstIterator: public LinkedArrayIterator<T> {
        private:
            /* Declare the LinkedArray class as friend. */
            friend class LinkedArray<T>;

        public:
            /* Constructor for the LinkedArrayConstIterator class, which takes the linked array where it should start (or nullptr to indicate end()). */
            LinkedArrayConstIterator(const LinkedArrayLink<T>* link): LinkedArrayIterator<T>(const_cast<LinkedArrayLink<T>*>(link)) {}

            /* Allows the iterator to be compared with the given one. */
            inline bool operator==(const LinkedArrayConstIterator& other) const { return LinkedArrayIterator<T>::operator==(other); }
            /* Allows the iterator to be compared with the given one, by inequality. */
            inline bool operator!=(const LinkedArrayConstIterator& other) const { return LinkedArrayIterator<T>::operator!=(other); }

            /* Increments the iterator by one (prefix). */
            inline LinkedArrayConstIterator& operator++() { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator++(); }
            /* Increments the iterator by one (postfix). */
            inline LinkedArrayConstIterator operator++(int) const { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator++(); }
            /* Decrements the iterator by one (prefix). */
            inline LinkedArrayConstIterator& operator--() { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator--(); }
            /* Decrements the iterator by one (postfix). */
            inline LinkedArrayConstIterator operator--(int) const { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator--(); }

            /* Increments the iterator by N steps. */
            inline LinkedArrayConstIterator& operator+=(SIZE_T N) { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator+=(N); }
            /* Returns a new iterator that is N steps ahead of this one. */
            inline LinkedArrayConstIterator operator+(SIZE_T N) const { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator+(N); }
            /* Decrements the iterator by N steps. */
            inline LinkedArrayConstIterator& operator-=(SIZE_T N) { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator-=(N); }
            /* Returns a new iterator that is N steps behind this one. */
            inline LinkedArrayConstIterator operator-(SIZE_T N) const { return (LinkedArrayConstIterator&) LinkedArrayIterator<T>::operator-(N); }

            /* Dereferences the iterator, but is deleted for the constant iterator. */
            T& operator*() = delete;

        };

        /* Reverse iterator for the LinkedArray class. */
        template <class T>
        class LinkedArrayReverseIterator: public LinkedArrayIterator<T> {
        private:
            /* Declare the LinkedArray class as friend. */
            friend class LinkedArray<T>;
            
        public:
            /* Constructor for the LinkedArrayReverseIterator class, which takes the linked array where it should start (or nullptr to indicate end()). */
            LinkedArrayReverseIterator(LinkedArrayLink<T>* link): LinkedArrayIterator<T>(link) {}

            /* Allows the iterator to be compared with the given one. */
            inline bool operator==(const LinkedArrayReverseIterator& other) const { return LinkedArrayIterator<T>::operator==(other); }
            /* Allows the iterator to be compared with the given one, by inequality. */
            inline bool operator!=(const LinkedArrayReverseIterator& other) const { return LinkedArrayIterator<T>::operator!=(other); }

            /* Increments the iterator by one (prefix). */
            inline LinkedArrayReverseIterator& operator++() { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator--(); }
            /* Increments the iterator by one (postfix). */
            inline LinkedArrayReverseIterator operator++(int) const { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator--(); }
            /* Decrements the iterator by one (prefix). */
            inline LinkedArrayReverseIterator& operator--() { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator++(); }
            /* Decrements the iterator by one (postfix). */
            inline LinkedArrayReverseIterator operator--(int) const { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator++(); }

            /* Increments the iterator by N steps. */
            inline LinkedArrayReverseIterator& operator+=(SIZE_T N) { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator-=(N); }
            /* Returns a new iterator that is N steps ahead of this one. */
            inline LinkedArrayReverseIterator operator+(SIZE_T N) const { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator-(N); }
            /* Decrements the iterator by N steps. */
            inline LinkedArrayReverseIterator& operator-=(SIZE_T N) { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator+=(N); }
            /* Returns a new iterator that is N steps behind this one. */
            inline LinkedArrayReverseIterator operator-(SIZE_T N) const { return (LinkedArrayReverseIterator&) LinkedArrayIterator<T>::operator+(N); }

        };

        /* Reverse constant iterator for the LinkedArray class. */
        template <class T>
        class LinkedArrayReverseConstantIterator: public LinkedArrayReverseIterator<T> {
        private:
            /* Declare the LinkedArray class as friend. */
            friend class LinkedArray<T>;
            
        public:
            /* Constructor for the LinkedArrayReverseConstantIterator class, which takes the linked array where it should start (or nullptr to indicate end()). */
            LinkedArrayReverseConstantIterator(const LinkedArrayLink<T>* link): LinkedArrayReverseIterator<T>(const_cast<LinkedArrayLink<T>*>(link)) {}

            /* Allows the iterator to be compared with the given one. */
            inline bool operator==(const LinkedArrayReverseConstantIterator& other) const { return LinkedArrayReverseIterator<T>::operator==(other); }
            /* Allows the iterator to be compared with the given one, by inequality. */
            inline bool operator!=(const LinkedArrayReverseConstantIterator& other) const { return LinkedArrayReverseIterator<T>::operator!=(other); }

            /* Increments the iterator by one (prefix). */
            inline LinkedArrayReverseConstantIterator& operator++() { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator++(); }
            /* Increments the iterator by one (postfix). */
            inline LinkedArrayReverseConstantIterator operator++(int) const { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator++(); }
            /* Decrements the iterator by one (prefix). */
            inline LinkedArrayReverseConstantIterator& operator--() { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator--(); }
            /* Decrements the iterator by one (postfix). */
            inline LinkedArrayReverseConstantIterator operator--(int) const { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator--(); }

            /* Increments the iterator by N steps. */
            inline LinkedArrayReverseConstantIterator& operator+=(SIZE_T N) { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator+=(N); }
            /* Returns a new iterator that is N steps ahead of this one. */
            inline LinkedArrayReverseConstantIterator operator+(SIZE_T N) const { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator+(N); }
            /* Decrements the iterator by N steps. */
            inline LinkedArrayReverseConstantIterator& operator-=(SIZE_T N) { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator-=(N); }
            /* Returns a new iterator that is N steps behind this one. */
            inline LinkedArrayReverseConstantIterator operator-(SIZE_T N) const { return (LinkedArrayReverseConstantIterator&) LinkedArrayReverseIterator<T>::operator-(N); }

            /* Dereferences the iterator, but is deleted for the constant iterator. */
            T& operator*() = delete;

        };

    };



    /* The LinkedArray class, which implements a LinkedList. */
    template <class T, class SIZE_T, bool D, bool C, bool M>
    class LinkedArray: public _array_intern::CopyControl<C> {
    public:
        /* The datatype stored in the array. */
        using type = T;
        /* The size type that is used in the array. */
        using size_type = SIZE_T;

        /* Muteable iterator for the LinkedArray class. */
        using iterator = _linked_array_intern::LinkedArrayIterator<T>;
        /* Immuteable iterator for the LinkedArray class. */
        using const_iterator = _linked_array_intern::LinkedArrayConstIterator<T>;
        /* Muteable reverse iterator for the LinkedArray class. */
        using reverse_iterator = _linked_array_intern::LinkedArrayReverseIterator<T>;
        /* Immuteable reverse iterator for the LinkedArray class. */
        using reverse_const_iterator = _linked_array_intern::LinkedArrayReverseConstantIterator<T>;

    private:
        /* The data stored in the LinkedArray. */
        _linked_array_intern::LinkedArrayStorage<T> storage;


        /* Private helper function that returns the node at the given index, going either forward or backward (whichever is shorter). */
        _linked_array_intern::LinkedArrayLink<T>* _node_at(SIZE_T index) const;
        /* Private helper function that actually appends a given element to the linked array. */
        void _push_back(_linked_array_intern::LinkedArrayLink<T>* new_link);
        /* Private helper function that actually prepends a given element to the linked array. */
        void _push_front(_linked_array_intern::LinkedArrayLink<T>* new_link);

    public:
        /* Default constructor for the LinkedArray, which initializes it to an empty list. */
        LinkedArray();
        /* Constructor for the LinkedArray class, which takes a single element and repeats that the given amount of times. Makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        LinkedArray(const T& elem, SIZE_T n_repeats);
        /* Constructor for the LinkedArray class, which takes a raw C-style vector to copy elements from and its size. Note that the Array's element type must have a copy custructor defined. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        LinkedArray(const T* list, SIZE_T list_size);
        /* Constructor for the LinkedArray class, which takes an initializer_list to initialize the LinkedArray with. Makes use of the element's copy constructor. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        LinkedArray(const std::initializer_list<T>& list);
        /* Constructor for the LinkedArray class, which takes a C++-style vector. Note that the Array's element type must have a copy custructor defined. */
        template <typename U = void, typename = std::enable_if_t<C, U>>
        LinkedArray(const std::vector<T>& list);

        /* Creates a new linked array that is a copy of this linked array with the elements in the given array copied and appended to them. Note that this requires the elements to be copy constructible. */
        template <typename U = LinkedArray>
        inline auto operator+(const LinkedArray& elems) const -> std::enable_if_t<M, U> { return LinkedArray(*this).operator+=(elems); }
        /* Creates a new linked array that is a copy of this linked array with the elements in the given array appended to them (moved). Does not require a move constructor to be defined. */
        inline LinkedArray& operator+(LinkedArray&& elems) const { return LinkedArray(*this).operator+=(std::move(elems)); }
        /* Adds a whole linked array worth of new elements to this linked array, copying them. Note that this requires the elements to be copy constructible. */
        template <typename U = LinkedArray&>
        auto operator+=(const LinkedArray& elems) -> std::enable_if_t<C, U>;
        /* Adds a whole linked array worth of new elements to this linked array, leaving the original array in an unused state (moving it). Does not require a move constructor to be defined. */
        LinkedArray& operator+=(LinkedArray&& elems);

        /* Adds a new element of type T to the start of the linked array, initializing it with its default constructor. Only needs a default constructor to be present. */
        template <typename U = LinkedArray&>
        auto push_front() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the start of the linked array, copying it. Note that this requires the element to be copy constructible. */
        template <typename U = LinkedArray&>
        auto push_front(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the start of the linked array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
        template <typename U = LinkedArray&>
        auto push_front(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the first element from the array. */
        LinkedArray& pop_front();

        /* Inserts a new element at the given location, pushing all elements coming after it one index back. Since we initialise the element with its default constructor, we need that to be present for the LinkedArray's element type. */
        template <typename U = LinkedArray&>
        auto insert(SIZE_T index) -> std::enable_if_t<D, U>;
        /* Inserts a copy of the given element at the given location, pushing all elements coming after it one index back. Requires the element to be copy constructible. */
        template <typename U = LinkedArray&>
        auto insert(SIZE_T index, const T& elem) -> std::enable_if_t<C, U>;
        /* Inserts the given element at the given location, pushing all elements coming after it one index back. Requires the element to be move constructible. */
        template <typename U = LinkedArray&>
        auto insert(SIZE_T index, T&& elem) -> std::enable_if_t<M, U>;

        /* Adds a new element of type T to the back of the linked array, initializing it with its default constructor. Only needs a default constructor to be present. */
        template <typename U = LinkedArray&>
        auto push_back() -> std::enable_if_t<D, U>;
        /* Adds a new element of type T to the back of the linked array, copying it. Note that this requires the element to be copy constructible. */
        template <typename U = LinkedArray&>
        auto push_back(const T& elem) -> std::enable_if_t<C, U>;
        /* Adds a new element of type T to the back of the linked array, leaving it in an usused state (moving it). Note that this requires the element to be move constructible. */
        template <typename U = LinkedArray&>
        auto push_back(T&& elem) -> std::enable_if_t<M, U>;
        /* Removes the last element from the array. */
        LinkedArray& pop_back();

        /* Erases an element with the given index from the linked array. Does nothing if the index is out-of-bounds. */
        LinkedArray& erase(SIZE_T index);
        /* Erases an element referenced by the given iterator. Does nothing if the index is out-of-bounds. */
        LinkedArray& erase(const iterator& iter);
        /* Erases an element referenced by the given constant iterator. Does nothing if the index is out-of-bounds. */
        inline LinkedArray& erase(const const_iterator& iter) { return this->erase((const iterator&) iter); }
        /* Erases an element referenced by the given reverse iterator. Does nothing if the index is out-of-bounds. */
        inline LinkedArray& erase(const reverse_iterator& iter) { return this->erase((const iterator&) iter); }
        /* Erases an element referenced by the given reverse, constant iterator. Does nothing if the index is out-of-bounds. */
        inline LinkedArray& erase(const reverse_const_iterator& iter) { return this->erase((const iterator&) iter); }
        /* Erases all elements until (and including) the element referenced by the given iterator. */
        LinkedArray& erase_until(const iterator& iter);
        /* Erases all elements until (and including) the element referenced by the given constant iterator. */
        inline LinkedArray& erase_until(const const_iterator& iter) { return this->erase_until((const iterator&) iter); }
        /* Erases all elements until (and including) the element referenced by the given reverse iterator. */
        inline LinkedArray& erase_until(const reverse_iterator& iter) { return this->erase_from((const iterator&) iter); }
        /* Erases all elements until (and including) the element referenced by the given reverse, constant iterator. */
        inline LinkedArray& erase_until(const reverse_const_iterator& iter) { return this->erase_from((const iterator&) iter); }
        /* Erases all elements from (and including) the element referenced by the given iterator. */
        LinkedArray& erase_from(const iterator& iter);
        /* Erases all elements from (and including) the element referenced by the given constant iterator. */
        inline LinkedArray& erase_from(const const_iterator& iter) { return this->erase_from((const iterator&) iter); }
        /* Erases all elements from (and including) the element referenced by the given reverse iterator. */
        inline LinkedArray& erase_from(const reverse_iterator& iter) { return this->erase_until((const iterator&) iter); }
        /* Erases all elements from (and including) the element referenced by the given reverse, constant iterator. */
        inline LinkedArray& erase_from(const reverse_const_iterator& iter) { return this->erase_until((const iterator&) iter); }
        /* Erases everything from the linked array, thus deallocating all its elements. */
        LinkedArray& reset();

        /* Resizes the linked array to the given size. Any leftover elements will be initialized with their default constructor (and thus requires the type to have one), and elements that won't fit will be deallocated. */
        template <typename U = LinkedArray&>
        auto resize(SIZE_T new_size) -> std::enable_if_t<D, U>;

        /* Returns a muteable reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        inline T& at(SIZE_T index) { return this->_node_at(index).value; }
        /* Returns a constant reference to the element at the given index. Performs in-of-bounds checks before accessing the element. */
        inline const T& at(SIZE_T index) const { return this->at(index); }
        /* Returns the first element in the list. */
        inline T& first() { return this->storage.head->value; }
        /* Returns the first element in the list. */
        inline const T& first() const { return this->storage.head->value; }
        /* Returns the last element in the list. */
        inline T& last() { return this->storage.tail->value; }
        /* Returns the last element in the list. */
        inline const T& last() const { return this->storage.tail->value; }

        /* Returns true if there are no elements in this array, or false otherwise. */
        inline bool empty() const { return this->storage.size == 0; }
        /* Returns the number of elements stored in this Array. */
        inline SIZE_T size() const { return this->storage.size; }

        /* Returns a muteable iterator to the beginning of the LinkedArray. */
        inline iterator begin() { return iterator(this->storage.head); }
        /* Returns a muteable iterator to the end of the LinkedArray. */
        inline iterator end() { return iterator(nullptr); }
        /* Returns an immuteable iterator to the beginning of the LinkedArray. */
        inline const_iterator begin() const { return const_iterator(this->storage.head); }
        /* Returns an immuteable iterator to the end of the LinkedArray. */
        inline const_iterator end() const { return const_iterator(nullptr); }
        /* Returns an immuteable iterator to the beginning of the LinkedArray. */
        inline const_iterator cbegin() const { return this->begin(); }
        /* Returns an immuteable iterator to the end of the LinkedArray. */
        inline const_iterator cend() const { return this->end(); }
        /* Returns a muteable iterator to the beginning of the LinkedArray. */
        inline reverse_iterator rbegin() { return reverse_iterator(this->storage.tail); }
        /* Returns a muteable reverse iterator to the end of the LinkedArray. */
        inline reverse_iterator rend() { return reverse_iterator(nullptr); }
        /* Returns an immuteable reverse iterator to the beginning of the LinkedArray. */
        inline reverse_const_iterator rbegin() const { return reverse_const_iterator(this->storage.tail); }
        /* Returns an immuteable reverse iterator to the end of the LinkedArray. */
        inline reverse_const_iterator rend() const { return reverse_const_iterator(nullptr); }
        /* Returns an immuteable reverse iterator to the beginning of the LinkedArray. */
        inline reverse_const_iterator crbegin() const { return this->rbegin(); }
        /* Returns an immuteable reverse iterator to the end of the LinkedArray. */
        inline reverse_const_iterator crend() const { return this->rend(); }

        /* Swap operator for the LinkedArray class. */
        friend void swap(LinkedArray& la1, LinkedArray& la2) {
            using std::swap;

            swap(la1.storage, la2.storage);
        }

    };
}

// Include the .cpp since it's all templated
#include "LinkedArray.cpp"

#endif
