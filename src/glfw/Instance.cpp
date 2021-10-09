/* INSTANCE.cpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 11:25:32
 * Last edited:
 *   06/10/2021, 11:25:32
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class for the GLFW library. Simply initializes
 *   and terminates it, and assigns a debug callback.
**/

#include "tools/Logger.hpp"
#include "glfw/Instance.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GLFW;


/***** HELPER FUNCTIONS *****/
/* Listens and reports GLFW errors. */
void glfw_error_callback(int code, const char* message) {
    // Simply throw them into the debugger
    logger.fatalc("GLFW", message, " (error code: ", code, ')');
}





/***** INSTANCE CLASS *****/
/* Constructor for the Instance class, which doesn't take anything. */
Instance::Instance() {
    // Initialize the GLFW library
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Assign the debug callback
    glfwSetErrorCallback(glfw_error_callback);

    // Get the list of attached monitors
    int n_monitors;
    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL) { logger.fatalc(Instance::channel, "Could not get primary monitor."); }
    GLFWmonitor** monitors = glfwGetMonitors(&n_monitors);
    if (monitors == NULL) { logger.fatalc(Instance::channel, "Could not get list of monitors."); }

    // Go through it to create enough Monitor instances
    this->monitors.reserve(static_cast<uint32_t>(n_monitors));
    for (uint32_t i = 0; i < static_cast<uint32_t>(n_monitors); i++) {
        // Create the instance
        this->monitors.push_back(GLFW::Monitor(monitors[i], i));

        // If this happens to be the primary monitor, set it as such
        if (monitors[i] == primary_monitor) {
            this->primary_monitor = &this->monitors[i];
        }
    }
}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    primary_monitor(other.primary_monitor),
    monitors(std::move(other.monitors))
{}

/* Destructor for the Instance class. */
Instance::~Instance() {
    glfwTerminate();
}



/* Returns a list of Vulkan extensions that should be enabled when using GLFW. */
Tools::Array<const char*> Instance::get_vulkan_extensions() {
    // We first collect a list of GLFW extensions
    uint32_t n_extensions = 0;
    const char** raw_extensions = glfwGetRequiredInstanceExtensions(&n_extensions);

    // Return them as an array
    return Tools::Array<const char*>(raw_extensions, n_extensions);
}



/* Swap operator for the Instance class. */
void GLFW::swap(Instance& i1, Instance& i2) {
    using std::swap;

    swap(i1.primary_monitor, i2.primary_monitor);
    swap(i1.monitors, i2.monitors);
}
