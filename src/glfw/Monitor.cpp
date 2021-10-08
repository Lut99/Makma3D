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
    this->_name = glfwGetMonitorName(this->glfw_monitor);
    this->_index = index;

    // Get the scale of the monitor
    glfwGetMonitorContentScale(this->glfw_monitor, &this->_xscale, &this->_yscale);
    
    // Get the video mode for the resolution
    const GLFWvidmode* video_mode = glfwGetVideoMode(this->glfw_monitor);
    this->_resolution.width  = static_cast<uint32_t>(video_mode->width / this->_xscale);
    this->_resolution.height = static_cast<uint32_t>(video_mode->height / this->_yscale);
    this->_scaled_resolution.width  = static_cast<uint32_t>(video_mode->width);
    this->_scaled_resolution.height = static_cast<uint32_t>(video_mode->height);
}
