/* INSTANCE.hpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 11:25:28
 * Last edited:
 *   06/10/2021, 11:25:28
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class for the GLFW library. Simply initializes
 *   and terminates it, and assigns a debug callback.
**/

#ifndef GLFW_INSTANCE_HPP
#define GLFW_INSTANCE_HPP

#include <glfw/glfw3.h>

#include "arrays/Array.hpp"
#include "window/Instance.hpp"

#include "Monitor.hpp"

namespace Makma3D::GLFW {
    /* The Instance class for the GLFW library. */
    class Instance: public Windowing::Instance {
    public:
        /* Logging channel name for the Instance class. */
        static constexpr const char* channel = "GLFWInstance";

    public:
        /* Constructor for the Instance class, which doesn't take anything. */
        Instance();
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Returns a list of Vulkan extensions that should be enabled when using GLFW. */
        virtual Tools::Array<const char*> get_vulkan_extensions() const;

        /* Returns the primary monitor. */
        virtual const const Windowing::Monitor* get_primary_monitor() const;
        /* Returns the list of monitors. */
        virtual const Tools::Array<const Windowing::Monitor*>& get_monitors() const;

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
