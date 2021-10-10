/* INSTANCE.hpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 11:19:00
 * Last edited:
 *   06/10/2021, 11:19:00
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class, which functions as the application-wide
 *   instance of the Makma3D library.
**/

#ifndef INSTANCE_INSTANCE_HPP
#define INSTANCE_INSTANCE_HPP

#include <string>

#include "arrays/Array.hpp"
#include "window/Instance.hpp"
#include "vulkanic/instance/Instance.hpp"

namespace Makma3D {
    /* The Instance class, which is the application-wide instance of the Makma3D library. */
    class Instance {
    public:
        /* Channel name for the Instance class. */
        static constexpr const char* channel = "Instance";

        /* The Window instance that we need. */
        const Windowing::Instance& window;
        /* The Vulkan instance that we need. */
        const Vulkanic::Instance& vulkanic;

    public:
        /* Constructor for the Instance class, which takes the Window instance for creating Windows and the Vulkanic Instance on which it relies. */
        Instance(const Windowing::Instance& window_instance, const Vulkanic::Instance& vulkanic_instance);
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Explicitly returns the VkInstance class of the internal Vulkan Instance. */
        inline const VkInstance& vk() const { return this->vulkanic.vk(); }
        /* Implicitly returns the VkInstance class of the internal Vulkan Instance. */
        inline operator const VkInstance&() const { return this->vulkanic.vk(); }

        /* Copy assignment operator for the Instance class, which is deleted. */
        Instance& operator=(const Instance& other) = delete;
        /* Move assignment operator for the Instance class. */
        inline Instance& operator=(Instance&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the Instance class. */
        friend void swap(Instance& i1, Instance& i2);

    };
    
    /* Swap operator for the Instance class. */
    void swap(Instance& i1, Instance& i2);

}

#endif
