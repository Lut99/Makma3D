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
#include <glfw/glfw3.h>
#include <vulkan/vulkan.h>

namespace Makma3D {
    /* The Monitor class, which is a virtual interface to the window API-specific wrapper around its own Monitor structs. */
    class Monitor {
    public:
        /* Logging channel name for the Monitor class. */
        static constexpr const char* channel = "Monitor";

    private:
        /* The GLFWMonitor struct we wrap. */
        GLFWmonitor* glfw_monitor;
        /* The GLFWvidmode struct that tells us how the monitor looks like 'without application' on it. */
        const GLFWvidmode* glfw_video_mode;

        /* The index of the monitor in the list of monitors. */
        uint32_t _index;
        /* The name of the monitor. */
        std::string _name;
        /* The scale of the monitor in the x-axis. */
        float _xscale;
        /* The scale of the monitor in the y-axis. */
        float _yscale;

        /* The position and size of the monitor (in screen coordinates). */
        VkRect2D _area;
        /* The position and size of the monitor's workarea (in screen coordinates). */
        VkRect2D _workarea;

    public:
        /* Constructor for the Monitor class.
         * @param glfw_monitor The GLFWMonitor object that we wrap.
         * @param index The index of the monitor in the list of monitors.
         */
        Monitor(GLFWmonitor* glfw_monitor, uint32_t index);

        /* Returns the current video mode of the monitor. */
        const GLFWvidmode* current_video_mode() const;
        /* Returns a pointer to the video mode of the monitor from before one of our applications was running on it. */
        inline const GLFWvidmode* idle_video_mode() const { return this->glfw_video_mode; }

        /* Returns the offset (in pixels) of the Monitor's screen in the global virtual screen.
         * Note that this operation is slower than getting the scaled equivalent, as that is the default that GLFW reports. */
        inline VkOffset2D offset() const { return { static_cast<int32_t>(this->_area.offset.x / this->_xscale), static_cast<int32_t>(this->_area.offset.y / this->_yscale) }; }
        /* Returns the resolution (in pixels) of the Monitor as a VkExtent2D.
         * Note that this operation is slower than getting the scaled equivalent, as that is the default that GLFW reports. */
        inline VkExtent2D resolution() const { return { static_cast<uint32_t>(this->_area.extent.width / this->_xscale), static_cast<uint32_t>(this->_area.extent.height / this->_yscale) }; }
        /* Returns the the workarea (in pixels) of the Monitor's screen in the global virtual screen.
         * Note that this operation is slower than getting the scaled equivalent, as that is the default that GLFW reports. */
        inline VkRect2D workarea() const { return { this->workarea_offset(), this->workarea_resolution() }; }
        /* Returns the offset of the workarea (in pixels) of the Monitor's screen in the global virtual screen.
         * Note that this operation is slower than getting the scaled equivalent, as that is the default that GLFW reports. */
        inline VkOffset2D workarea_offset() const { return { static_cast<int32_t>(this->_workarea.offset.x / this->_xscale), static_cast<int32_t>(this->_workarea.offset.y / this->_yscale) }; }
        /* Returns the resolution (in pixels) of the workarea of the Monitor as a VkExtent2D.
         * Note that this operation is slower than getting the scaled equivalent, as that is the default that GLFW reports. */
        inline VkExtent2D workarea_resolution() const { return { static_cast<uint32_t>(this->_workarea.extent.width / this->_xscale), static_cast<uint32_t>(this->_workarea.extent.height / this->_yscale) }; }

        /* Returns the offset (in screen coordinates) of the Monitor's screen in the global virtual screen. */
        inline const VkOffset2D& scaled_offset() const { return this->_area.offset; }
        /* Returns the resolution (in screen coordinates) of the Monitor as a VkExtent2D. */
        inline const VkExtent2D& scaled_resolution() const { return this->_area.extent; }
        /* Returns the the workarea (in screen coordinates) of the Monitor's screen in the global virtual screen. */
        inline const VkRect2D& scaled_workarea() const { return this->_workarea; }
        /* Returns the offset of the workarea (in screen coordinates) of the Monitor's screen in the global virtual screen. */
        inline const VkOffset2D& scaled_workarea_offset() const { return this->_workarea.offset; }
        /* Returns the resolution of the workarea (in screen coordinates) of the Monitor as a VkExtent2D. */
        inline const VkExtent2D& scaled_workarea_resolution() const { return this->_workarea.extent; }

        /* Returns the index of the Monitor. */
        inline uint32_t index() const { return this->_index; }
        /* Returns the name of the Monitor. */
        inline const std::string& name() const { return this->_name; }
        /* Returns the scale on the X-axis in which the monitor currently runs. */
        inline float xscale() const { return this->_xscale; }
        /* Returns the scale on the Y-axis in which the monitor currently runs. */
        inline float yscale() const { return this->_yscale; }

        /* Explicitly returns the internal GLFWmonitor pointer. */
        inline GLFWmonitor* const& glfw() const { return this->glfw_monitor; }
        /* Implicitly returns the internal GLFWmonitor pointer. */
        inline operator GLFWmonitor* const&() const { return this->glfw_monitor; }

    };
}

#endif
