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

#include <glfw/glfw3.h>

#include "tools/Logger.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "instance/Instance.hpp"

using namespace std;
using namespace Makma3D;


/***** INSTANCE CLASS *****/
/* The version of the Makma3D engine. */
const Version Instance::version(0, 1, 0);

/* Constructor for the Instance class. */
Instance::Instance(const std::string& application_name, const Version& application_version, const Tools::Array<Extension>& extensions) {
    logger.logc(Verbosity::important, Instance::channel, "Initializing Makma3D...");

    /* EXTENSION COLLECTION */
    // The list of base extensions to enable
    Tools::Array<const char*> vk_extensions = {};
    // The list of base layers to enable
    Tools::Array<const char*> vk_layers = {};

    // Per Makma3D extension, pad this list
    for (uint32_t i = 0; i < extensions.size(); i++) {
        // If already done, skip
        if (this->extensions.find(extensions[i]) != this->extensions.end()) {
            logger.warningc(Instance::channel, "Skipping duplicate extension '", extension_names[(int) extensions[i]], "'.");
            continue;
        }

        // Add the appropriate vulkan extensions & layers
        switch(extensions[i]) {
        case Extension::debug:
            vk_extensions += { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
            vk_layers     += { "VK_LAYER_KHRONOS_validation" };
            break;
        
        default:
            logger.fatalc(Instance::channel, "Cannot enable unsupported extension '", extension_names[(int) extensions[i]], "'.");

        }

        // Mark this extension as Enabled
        this->extensions.insert(extensions[i]);
        logger.logc(Verbosity::debug, Instance::channel, "Enabled Makma3D extension '", extension_names[(int) extensions[i]], "'.");
    }



    /* BACKEND INITIALIZATION */
    // Initialize GLFW first
    this->glfw_instance.init();
    // Then do Vulkan
    this->vk_instance.init(application_name.c_str(), application_version.vk(), Instance::version.vk(), vk_extensions + this->glfw_instance.get_vulkan_extensions(), vk_layers);

    // Next, if given, enable the debug parts of the backend
    if (this->extension_enabled(Extension::debug)) {
        this->glfw_instance.init_debug();
        this->vk_instance.init_debug();
    }



    /* DONE */
    logger.logc(Verbosity::important, Instance::channel, "Initialization complete.");
}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    extensions(std::move(other.extensions)),
    glfw_instance(std::move(other.glfw_instance)),
    vk_instance(std::move(other.vk_instance))
{}

/* Destructor for the Instance class. */
Instance::~Instance() {}



/* Returns a list of enabled Extensions that can be iterated through. */
Tools::Array<Extension> Instance::get_extensions() const {
    Tools::Array<Extension> result(static_cast<uint32_t>(this->extensions.size()));
    for (Extension ext : this->extensions) {
        result.push_back(ext);
    }
    return result;
}



/* Returns the list of (supported) PhysicalDevices that are registered to the Vulkan backend. */
Tools::Array<GPU::PhysicalDevice> Instance::get_physical_devices(const Windowing::Window& window) const {
    // First, compile a list of device extensions & features to enable based on the enabled Makma3D extensions
    Tools::Array<const char*> vk_device_extensions = {};
    Tools::Array<Vulkanic::DeviceFeature> vk_device_features = { Vulkanic::DeviceFeature::anisotropy };
    for (uint32_t i = 0; i < this->extensions.size(); i++) {
        /* None, as of yet */
    }

    // Call the Vulkan instance's version of this function
    return this->vk_instance.get_physical_devices(window.surface().vk(), vk_device_extensions, vk_device_features);
}



/* Swap operator for the Instance class. */
void Makma3D::swap(Instance& i1, Instance& i2) {
    using std::swap;

    swap(i1.extensions, i2.extensions);
    swap(i1.glfw_instance, i2.glfw_instance);
    swap(i1.vk_instance, i2.vk_instance);
}
