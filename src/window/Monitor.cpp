/* MONITOR.cpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:58:32
 * Last edited:
 *   08/10/2021, 14:58:32
 * Auto updated?
 *   Yes
 *
 * Description:
 *   <Todo>
**/

#include "tools/Logger.hpp"
#include "window/Monitor.hpp"

using namespace std;
using namespace Makma3D;


/***** MONITOR CLASS *****/
/* Constructor for the Monitor class. */
Monitor::Monitor(GLFWmonitor* glfw_monitor, uint32_t index) :
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

    // Store the position and resolution in the area
    int x, y;
    glfwGetMonitorPos(this->glfw_monitor, &x, &y);
    this->_area.offset = { static_cast<int32_t>(x), static_cast<int32_t>(y) };
    this->_area.extent = { static_cast<uint32_t>(this->glfw_video_mode->width), static_cast<uint32_t>(this->glfw_video_mode->height) };

    // Store the workarea data in the workarea
    int w, h;
    glfwGetMonitorWorkarea(this->glfw_monitor, &x, &y, &w, &h);
    this->_workarea.offset = { static_cast<int32_t>(x), static_cast<int32_t>(y) };
    this->_workarea.extent = { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
}



/* Returns the current video mode of the monitor. */
const GLFWvidmode* Monitor::current_video_mode() const {
    const GLFWvidmode* result = glfwGetVideoMode(this->glfw_monitor);
    if (result == NULL) { logger.fatalc(Monitor::channel, "Could not get current video mode."); }
    return result;
}
