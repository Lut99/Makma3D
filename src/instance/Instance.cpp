/* INSTANCE.cpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 11:19:23
 * Last edited:
 *   06/10/2021, 11:19:23
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class, which functions as the application-wide
 *   instance of the Makma3D library.
**/

#include "tools/Logger.hpp"
#include "instance/Instance.hpp"

using namespace std;
using namespace Makma3D;


/***** INSTANCE CLASS *****/
/* Constructor for the Instance class, which takes the Window instance for creating Windows and the Vulkanic Instance on which it relies. */
Instance::Instance(const Windowing::Instance& window_instance, const Vulkanic::Instance& vulkanic_instance) :
    window(window_instance),
    vulkanic(vulkanic_instance)
{}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    window(other.window),
    vulkanic(other.vulkanic)
{}

/* Destructor for the Instance class. */
Instance::~Instance() {}



/* Swap operator for the Instance class. */
void Makma3D::swap(Instance& i1, Instance& i2) {
    #ifndef NDEBUG
    if (&i1.window != &i2.window) { logger.fatalc(Instance::channel, "Cannot swap makma instances with different window instances."); }
    if (&i1.vulkanic != &i2.vulkanic) { logger.fatalc(Instance::channel, "Cannot swap makma instances with different vulkanic instances."); }
    #endif

    using std::swap;
}
