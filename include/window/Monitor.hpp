/* MONITOR.hpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:52:36
 * Last edited:
 *   08/10/2021, 14:52:36
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Monitor class, which is a virtual interface to wrap
 *   around the window API's monitor classes and provide its information.
**/

#ifndef WINDOW_MONITOR_HPP
#define WINDOW_MONITOR_HPP

#include <string>
#include <vulkan/vulkan.h>

namespace Makma3D::Windowing {
    /* The Monitor class, which is a virtual interface to the window API-specific wrapper around its own Monitor structs. */
    class Monitor {
    protected:
        /* The index of the monitor in the list of monitors. */
        uint32_t _index;
        /* The name of the monitor. */
        std::string _name;
        /* The scale of the monitor in the x-axis. */
        float _xscale;
        /* The scale of the monitor in the y-axis. */
        float _yscale;
        /* The hardware resolution of the monitor (as a VkExtent2D). */
        VkExtent2D _resolution;
        /* The scaled resolution of the monitor (as a VkExtent2D). */
        VkExtent2D _scaled_resolution;


        /* Constructor for the Monitor class, which doesn't take anything - meaning initialization is left to its derived children. */
        Monitor();

    public:
        /* Virtual destructor for the Monitor class, which is trivial. */
        virtual ~Monitor() = default;

        /* Returns the index of the Monitor. */
        inline uint32_t index() const { return this->_index; }
        /* Returns the name of the Monitor. */
        inline const std::string& name() const { return this->_name; }
        /* Returns the scale on the X-axis in which the monitor currently runs. */
        inline float xscale() const { return this->_xscale; }
        /* Returns the scale on the Y-axis in which the monitor currently runs. */
        inline float yscale() const { return this->_yscale; }
        /* Returns the resolution of the Monitor as a VkExtent2D. */
        inline const VkExtent2D& resolution() const { return this->_resolution; }
        /* Returns the scaled resolution of the Monitor as a VkExtent2D. */
        inline const VkExtent2D& scaled_resolution() const { return this->_scaled_resolution; }

        /* Allows the monitor to be copied polymorphically. */
        virtual Monitor* copy() const = 0;

    };
}

#endif
