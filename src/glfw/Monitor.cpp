/* MONITOR.cpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:51:07
 * Last edited:
 *   08/10/2021, 14:51:07
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Monitor class, which wraps around a GLFW monitor struct
 *   and lists its properties.
**/

#include "tools/Logger.hpp"
#include "glfw/Monitor.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GLFW;


/***** MONITOR CLASS *****/
/* Constructor for the Monitor class, which takes the GLFWmonitor it wraps and its index. */
Monitor::Monitor(GLFWmonitor* glfw_monitor, uint32_t index) :
    Windowing::Monitor(),
    glfw_monitor(glfw_monitor)
{
    // Extract the properties from the struct
    const char* name = glfwGetMonitorName(this->glfw_monitor);
    if (name == NULL) { logger.fatalc(Monitor::channel, "Could not get monitor name."); }
    this->_name = name;
    this->_index = index;

    // Get the scale of the monitor
    glfwGetMonitorContentScale(this->glfw_monitor, &this->_xscale, &this->_yscale);
    
    // Get the video mode for the resolution
    this->glfw_video_mode = glfwGetVideoMode(this->glfw_monitor);
    if (this->glfw_video_mode == NULL) { logger.fatalc(Monitor::channel, "Could not get idle video mode."); }

    // Compute the resolution(s)
    this->_resolution.width  = static_cast<uint32_t>(this->glfw_video_mode->width / this->_xscale);
    this->_resolution.height = static_cast<uint32_t>(this->glfw_video_mode->height / this->_yscale);
    this->_scaled_resolution.width  = static_cast<uint32_t>(this->glfw_video_mode->width);
    this->_scaled_resolution.height = static_cast<uint32_t>(this->glfw_video_mode->height);
}



/* Returns the middle of the workarea of the monitor. */
VkOffset2D Monitor::get_center() const {
    // Get the monitor's workarea
    int x, y, w, h;
    glfwGetMonitorWorkarea(this->glfw_monitor, &x, &y, &w, &h);

    // Return the center
    return {
        static_cast<int32_t>(x + w / 2),
        static_cast<int32_t>(y + h / 2)
    };
}



/* Returns the current video mode of the monitor. */
const GLFWvidmode* Monitor::current_video_mode() const {
    const GLFWvidmode* result = glfwGetVideoMode(this->glfw_monitor);
    if (result == NULL) { logger.fatalc(Monitor::channel, "Could not get current video mode."); }
    return result;
}
