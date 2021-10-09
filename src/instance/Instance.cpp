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

#include "instance/Instance.hpp"

using namespace std;
using namespace Makma3D;


/***** INSTANCE CLASS *****/
/* Constructor for the Instance class, which takes the application name, the application version (created with VK_MAKE_VERSION), a list of Vulkan extensions to enable and a list of Vulkan layers to enable. If NDEBUG isn't defined, the Vulkan debug extension & layers are automatically enabled. */
Instance::Instance(const std::string& application_name, uint32_t application_version, const Tools::Array<const char*>& vulkan_extensions, const Tools::Array<const char*>& vulkan_layers) :
    _glfw_instance(),
    _vulkanic_instance(application_name, application_version, vulkan_extensions, vulkan_layers)
{}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    _glfw_instance(std::move(other._glfw_instance)),
    _vulkanic_instance(std::move(other._vulkanic_instance))
{}

/* Destructor for the Instance class. */
Instance::~Instance() {}



/* Swap operator for the Instance class. */
void Makma3D::swap(Instance& i1, Instance& i2) {
    using std::swap;

    swap(i1._glfw_instance, i2._glfw_instance);
    swap(i1._vulkanic_instance, i2._vulkanic_instance);
}
