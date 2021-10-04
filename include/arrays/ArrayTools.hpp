/* ARRAY TOOLS.hpp
 *   by Lut99
 *
 * Created:
 *   04/10/2021, 16:39:29
 * Last edited:
 *   04/10/2021, 16:39:29
 * Auto updated?
 *   Yes
 *
 * Description:
 *   File that contains a lot of basis for the various Array classes.
**/

#ifndef TOOLS_ARRAY_TOOLS_HPP
#define TOOLS_ARRAY_TOOLS_HPP

namespace Makma3D::Tools::_array_intern {
    /* The CopyControl class, which uses template specializations to select an appropriate set of copy constructors for any array class.
     * Note that this works because we're utilizing the fact that the compiler cannot generate the standard copy constructor/assignment operator for Array if it doesn't exist in its parent - which we control here. This way, we avoid specializating the entire array class. */
    template <bool C>
    class CopyControl;

    template <>
    class CopyControl<false> {
    public:
        /* Default constructor for the CopyControl class. */
        CopyControl() = default;

        /* Copy constructor for the CopyControl class. */
        CopyControl(const CopyControl&) = delete;
        /* Copy assignment for the CopyControl class. */
        CopyControl& operator=(const CopyControl&) = delete;

        /* Move constructor for the CopyControl class. */
        CopyControl(CopyControl&&) = default;
        /* Move assignment for the CopyControl class. */
        CopyControl& operator=(CopyControl&&) = default;
    };

    template <>
    class CopyControl<true> {
    public:
        /* Default constructor for the CopyControl class. */
        CopyControl() = default;

        /* Copy constructor for the CopyControl class. */
        CopyControl(const CopyControl&) = default;
        /* Copy assignment for the CopyControl class. */
        CopyControl& operator=(const CopyControl&) = default;

        /* Move constructor for the CopyControl class. */
        CopyControl(CopyControl&&) = default;
        /* Move assignment for the CopyControl class. */
        CopyControl& operator=(CopyControl&&) = default;
    };



    /* The CopyMoveControl class, which uses template specializations to select an appropriate set of copy & move constructors for any array class.
     * Note that this works because we're utilizing the fact that the compiler cannot generate the standard copy/move constructor/assignment operator for Array if it doesn't exist in its parent - which we control here. This way, we avoid specializating the entire array class. */
    template <bool C, bool M>
    class CopyMoveControl;

    template <>
    class CopyMoveControl<false, false> {
    public:
        /* Default constructor for the CopyMoveControl class. */
        CopyMoveControl() = default;

        /* Copy constructor for the CopyMoveControl class. */
        CopyMoveControl(const CopyMoveControl&) = delete;
        /* Copy assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(const CopyMoveControl&) = delete;

        /* Move constructor for the CopyMoveControl class. */
        CopyMoveControl(CopyMoveControl&&) = delete;
        /* Move assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(CopyMoveControl&&) = delete;
    };

    template <>
    class CopyMoveControl<false, true> {
    public:
        /* Default constructor for the CopyMoveControl class. */
        CopyMoveControl() = default;

        /* Copy constructor for the CopyMoveControl class. */
        CopyMoveControl(const CopyMoveControl&) = delete;
        /* Copy assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(const CopyMoveControl&) = delete;

        /* Move constructor for the CopyMoveControl class. */
        CopyMoveControl(CopyMoveControl&&) = default;
        /* Move assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(CopyMoveControl&&) = default;
    };

    template <>
    class CopyMoveControl<true, false> {
    public:
        /* Default constructor for the CopyMoveControl class. */
        CopyMoveControl() = default;

        /* Copy constructor for the CopyMoveControl class. */
        CopyMoveControl(const CopyMoveControl&) = default;
        /* Copy assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(const CopyMoveControl&) = default;

        /* Move constructor for the CopyMoveControl class. */
        CopyMoveControl(CopyMoveControl&&) = delete;
        /* Move assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(CopyMoveControl&&) = delete;
    };

    template <>
    class CopyMoveControl<true, true> {
    public:
        /* Default constructor for the CopyMoveControl class. */
        CopyMoveControl() = default;

        /* Copy constructor for the CopyMoveControl class. */
        CopyMoveControl(const CopyMoveControl&) = default;
        /* Copy assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(const CopyMoveControl&) = default;

        /* Move constructor for the CopyMoveControl class. */
        CopyMoveControl(CopyMoveControl&&) = default;
        /* Move assignment for the CopyMoveControl class. */
        CopyMoveControl& operator=(CopyMoveControl&&) = default;
    };



    /* The ArrayStorage class, which basically implements the Array's standard operators for a heap-allocated array with size and a capacity. */
    template <class T, class SIZE_T>
    class ArrayStorage {
    public:
        /* Pointer to the elements stored in the Array. */
        T* elements;
        /* The number of elements we have currently stored. */
        SIZE_T size;
        /* The number of elements we have allocated space for. */
        SIZE_T capacity;

    public:
        /* Default constructor for the ArrayStorage class, which initializes itself to 0. */
        ArrayStorage();
        /* Copy constructor for the ArrayStorage class. */
        ArrayStorage(const ArrayStorage& other);
        /* Move constructor for the ArrayStorage class. */
        ArrayStorage(ArrayStorage&& other);
        /* Destructor for the ArrayStorage class. */
        ~ArrayStorage();

        /* Copy assignment operator for the ArrayStorage class */
        inline ArrayStorage& operator=(const ArrayStorage& other) { return *this = ArrayStorage(other); }
        /* Move assignment operator for the ArrayStorage class */
        inline ArrayStorage& operator=(ArrayStorage&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the ArrayStorage class. */
        friend void swap(ArrayStorage& as1, ArrayStorage& as2) {
            using std::swap;

            swap(as1.elements, as2.elements);
            swap(as1.size, as2.size);
            swap(as1.capacity, as2.capacity);
        }

    };



    /* The StackArrayStorage class, which basically implements the StackArray's standard operators for a stack-allocated array with its size. */
    template <class T, size_t SIZE, class SIZE_T>
    class StackArrayStorage {
    public:
        /* The elements stored in the StackArray. */
        T elements[SIZE];
        /* The number of elements we have currently stored. */
        SIZE_T size;
        /* The number of elements we have allocated space for. */
        static constexpr const SIZE_T capacity = static_cast<SIZE_T>(SIZE);

    public:
        /* Default constructor for the StackArrayStorage class, which initializes itself to 0. */
        StackArrayStorage();
        /* Copy constructor for the StackArrayStorage class. */
        StackArrayStorage(const StackArrayStorage& other);
        /* Move constructor for the StackArrayStorage class. */
        StackArrayStorage(StackArrayStorage&& other);
        /* Destructor for the StackArrayStorage class. */
        ~StackArrayStorage();

        /* Copy assignment operator for the StackArrayStorage class */
        inline StackArrayStorage& operator=(const StackArrayStorage& other) { return *this = StackArrayStorage(other); }
        /* Move assignment operator for the StackArrayStorage class */
        inline StackArrayStorage& operator=(StackArrayStorage&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the StackArrayStorage class. */
        friend void swap(StackArrayStorage& as1, StackArrayStorage& as2) {
            using std::swap;

            swap(as1.elements, as2.elements);
            swap(as1.size, as2.size);
        }

    };
}

#endif
