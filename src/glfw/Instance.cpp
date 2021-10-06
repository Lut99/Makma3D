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
#include "Instance.hpp"

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
}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) {}

/* Destructor for the Instance class. */
Instance::~Instance() {
    glfwTerminate();
}



/* Swap operator for the Instance class. */
void GLFW::swap(Instance& i1, Instance& i2) {}
