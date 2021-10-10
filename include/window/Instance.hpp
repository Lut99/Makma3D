/* INSTANCE.hpp
 *   by Lut99
 *
 * Created:
 *   10/10/2021, 12:09:11
 * Last edited:
 *   10/10/2021, 12:09:11
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance baseclass for all Window API instances.
**/

#ifndef WINDOW_INSTANCE_HPP
#define WINDOW_INSTANCE_HPP

#include <arrays/Array.hpp>

#include "Monitor.hpp"

namespace Makma3D::Windowing {
    /* The Instance class, which forms the baseclass of all API backend Instances. */
    class Instance {
    protected:
        /* The primary monitor as told to us by the OS. */
        const Windowing::Monitor* primary_monitor;
        /* List of Monitor instances created with the backend. */
        Tools::Array<const Windowing::Monitor*> monitors;


        /* Protected constructor for the Instance class, which doesn't do anything except exist. */
        Instance();

    public:
        /* Virtual destructor for the Instance class, which is trivial. */
        virtual ~Instance() = default;

        /* Returns a list of Vulkan extensions that should be enabled when using GLFW. */
        virtual Tools::Array<const char*> get_vulkan_extensions() const = 0;

        /* Returns the primary monitor. */
        virtual const Windowing::Monitor* get_primary_monitor() const = 0;
        /* Returns the list of monitors. */
        virtual const Tools::Array<const Windowing::Monitor*>& get_monitors() const = 0;

    };
}

#endif
