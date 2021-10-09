/* MONITOR.hpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:51:04
 * Last edited:
 *   08/10/2021, 14:51:04
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Monitor class, which wraps around a GLFW monitor struct
 *   and lists its properties.
**/

#ifndef GLFW_MONITOR_HPP
#define GLFW_MONITOR_HPP

#include <glfw/glfw3.h>

#include "window/Monitor.hpp"

namespace Makma3D::GLFW {
    /* The Monitor class, which extends from the base monitor to implement its properties with GLFW. */
    class Monitor: public Windowing::Monitor {
    public:
        /* Logging channel name for the Monitor class. */
        static constexpr const char* channel = "GLFWMonitor";

    private:
        /* The GLFWMonitor struct we wrap. */
        GLFWmonitor* glfw_monitor;
        /* The GLFWvidmode struct that tells us how the monitor looks like 'without application' on it. */
        const GLFWvidmode* glfw_video_mode;
    
    public:
        /* Constructor for the Monitor class, which takes the GLFWmonitor it wraps and its index. */
        Monitor(GLFWmonitor* glfw_monitor, uint32_t index);

        /* Returns the current video mode of the monitor. */
        const GLFWvidmode* current_video_mode() const;
        /* Returns a pointer to the video mode of the monitor from before one of our applications was running on it. */
        inline const GLFWvidmode* idle_video_mode() const { return this->glfw_video_mode; }
        /* Explicitly returns the internal GLFWmonitor pointer. */
        inline GLFWmonitor* const& glfw() const { return this->glfw_monitor; }
        /* Implicitly returns the internal GLFWmonitor pointer. */
        inline operator GLFWmonitor* const&() const { return this->glfw_monitor; }

        /* Allows the monitor to be copied polymorphically. */
        virtual Monitor* copy() const { return new Monitor(*this); }

    };
}

#endif
