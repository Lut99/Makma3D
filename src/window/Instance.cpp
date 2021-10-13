/* INSTANCE.cpp
 *   by Lut99
 *
 * Created:
 *   10/10/2021, 12:09:14
 * Last edited:
 *   10/10/2021, 12:09:14
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance baseclass for all Window API instances.
**/

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include "tools/Logger.hpp"

#include "window/Instance.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GLFW;


/***** CONSTANTS *****/
/* Channel name for GLFW messages. */
static constexpr const char* glfw_channel = "GLFW";





/***** CALLBACKS *****/
/* Listens to and reports GLFW errors. */
void glfw_error_callback(int code, const char* message) {
    // Simply throw them into the debugger
    logger.fatalc(glfw_channel, message, " (error code: ", code, ')');
}





/***** INSTANCE CLASS *****/
/* Constructor for the Instance class. */
Instance::Instance() {}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    _primary(other._primary),
    _monitors(std::move(other._monitors))
{}

/* Destructor for the Instance class. */
Instance::~Instance() {
    // Destroy the windows
    for (uint32_t i = 0; i < this->_monitors.size(); i++) {
        delete this->_monitors[i];
    }

    // Destroy the GLFW library
    glfwTerminate();
}



/* Initializes the instance. */
void Instance::init() {
    // Initialize the GLFW library
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Next, initialize all monitors, so get a list of 'em from GLFW
    int n_monitors;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    GLFWmonitor** monitors = glfwGetMonitors(&n_monitors);
    if (monitors == nullptr) {
        logger.fatalc(Instance::channel, "Could not get list of GLFW monitors.");
    }

    // Create a Monitor class for each of them and store them internally
    this->_monitors.reserve(static_cast<uint32_t>(n_monitors));
    for (int i = 0; i < n_monitors; i++) {
        this->_monitors.push_back(new Monitor(monitors[i], static_cast<uint32_t>(i)));
        if (this->_monitors.last()->glfw() == primary) {
            this->_primary = this->_monitors.last();
        }
    }
}

/* Initializes the debugging part of the instance. */
void Instance::init_debug() {
    logger.logc(Verbosity::details, Instance::channel, "Enabling GLFW debugger...");

    // Simply set the GLFW callback
    glfwSetErrorCallback(glfw_error_callback);
}



/* Returns the list of Vulkan extensions as required by GLFW. */
Tools::Array<const char*> Instance::get_vulkan_extensions() const {
    // We first collect a list of GLFW extensions
    uint32_t n_extensions = 0;
    const char** raw_extensions = glfwGetRequiredInstanceExtensions(&n_extensions);

    // Return them as an array
    return Tools::Array<const char*>(raw_extensions, n_extensions);
}



/* Swap operator for the Instance class. */
void GLFW::swap(Instance& i1, Instance& i2) {
    using std::swap;

    swap(i1._primary, i2._primary);
    swap(i1._monitors, i2._monitors);
}
