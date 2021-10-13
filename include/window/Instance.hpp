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

namespace Makma3D::GLFW {
    /* The Instance class, which forms the baseclass of all API backend Instances. */
    class Instance {
    public:
        /* Channel name for the Instance class. */
        static constexpr const char* channel = "WindowInstance";

    private:
        /* Reference to the Primary monitor in the Instance. */
        const Monitor* _primary;
        /* The list of monitors as reported by GLFW. */
        Tools::Array<const Monitor*> _monitors;

    public:
        /* Constructor for the Instance class. */
        Instance();
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Initializes the instance. */
        void init();
        /* Initializes the debugging part of the instance. */
        void init_debug();

        /* Returns the list of Vulkan extensions as required by GLFW. */
        Tools::Array<const char*> get_vulkan_extensions() const;

        /* Returns the primary monitor as given by GLFW. */
        inline const Monitor* get_primary_monitor() const { return this->_primary; }
        /* Returns the list of available monitors as given by GLFW. */
        inline const Tools::Array<const Monitor*>& get_monitors() const { return this->_monitors; }

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
