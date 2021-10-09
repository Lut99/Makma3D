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

#include "Monitor.hpp"

namespace Makma3D::GLFW {
    /* The Instance class for the GLFW library. */
    class Instance {
    public:
        /* Logging channel name for the Instance class. */
        static constexpr const char* channel = "GLFWInstance";

    private:
        /* The primary monitor as told to us by the OS. */
        GLFW::Monitor* primary_monitor;
        /* List of Monitor instances created with GLFW. */
        Tools::Array<GLFW::Monitor> monitors;

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
        static Tools::Array<const char*> get_vulkan_extensions();

        /* Returns the primary monitor. */
        inline const GLFW::Monitor& get_primary_monitor() const { return *this->primary_monitor; }
        /* Returns the list of monitors. */
        inline const Tools::Array<GLFW::Monitor>& get_monitors() const { return this->monitors; }

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
