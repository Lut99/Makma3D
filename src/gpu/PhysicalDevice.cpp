/* PHYSICAL DEVICE.cpp
 *   by Lut99
 *
 * Created:
 *   13/10/2021, 16:38:35
 * Last edited:
 *   13/10/2021, 16:38:35
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the PhysicalDevice class, which represents a single,
 *   supported GPU that is registered in the Vulkan instance.
**/

#include "tools/Logger.hpp"

#include "gpu/PhysicalDevice.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GPU;


/***** HELPER FUNCTIONS *****/
/* Returns whether or not the given physical device supports the required list of device extensions.
 * @param vk_physical_device The VkPhysicalDevice which we want to check.
 * @param vk_device_extensions The list of Vulkan device extensions which the device should support at the least.
 * @returns Whether or not the given device supports the extensions (true) or not (false). */
static bool gpu_supports_extensions(const VkPhysicalDevice& vk_physical_device, const Tools::Array<const char*>& vk_device_extensions) {
    VkResult vk_result;

    // Get the physical device properties for debugging
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(vk_physical_device, &properties);

    // Get a list of all extensions supported on this device
    uint32_t n_supported_extensions = 0;
    if ((vk_result = vkEnumerateDeviceExtensionProperties(vk_physical_device, nullptr, &n_supported_extensions, nullptr)) != VK_SUCCESS) {
        logger.warningc(PhysicalDevice::channel, "Could not get the number of supported extensions on physical device '", properties.deviceName, "'; assuming nothing supported.");
        return vk_device_extensions.empty();
    }
    Tools::Array<VkExtensionProperties> supported_extensions(n_supported_extensions);
    if ((vk_result = vkEnumerateDeviceExtensionProperties(vk_physical_device, nullptr, &n_supported_extensions, supported_extensions.wdata(n_supported_extensions))) != VK_SUCCESS) {
        logger.warningc(PhysicalDevice::channel, "Could not get the supported extensions on physical device '", properties.deviceName, "'; assuming nothing supported.");
        return vk_device_extensions.empty();
    }

    // Now simply make sure that all extensions in the list appear in the retrieved list
    for (uint32_t i = 0; i < vk_device_extensions.size(); i++) {
        const char* extension = vk_device_extensions[i];

        // Try to find it in the list of supported extensions
        bool found = false;
        for (uint32_t j = 0; j < supported_extensions.size(); j++) {
            if (strcmp(extension, supported_extensions[j].extensionName) == 0) {
                // Dope, continue
                found = true;
                break;
            }
        }
        if (!found) { return false; }
    }

    // However, if all extensions passed the test, we're done
    return true;
}

/* Returns whether or not the given physical device supports the required list of device features.
 * @param vk_physical_device The VkPhysicalDevice which we want to check.
 * @param vk_device_features List of Vulkan features that we'd like to have supported.
 * @returns Whether or not the device supports the given features (true) or not (false). */
static bool gpu_supports_features(const VkPhysicalDevice& vk_physical_device, const Tools::Array<Vulkanic::DeviceFeature>& vk_device_features) {
    // Get the device features
    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(vk_physical_device, &supported_features);

    // Now loop through the features to find if they are supported
    for (uint32_t i = 0; i < vk_device_features.size(); i++) {
        switch(vk_device_features[i]) {
        case Vulkanic::DeviceFeature::anisotropy:
            if (!supported_features.samplerAnisotropy) { return false; }
        
        default:
            logger.warningc(PhysicalDevice::channel, "Encountered unsupported device feature '", Vulkanic::device_feature_names[(int) vk_device_features[i]], "'.");

        }
    }

    // If we made it through, everything's supported
    return true;
}





/***** PHYSICALDEVICE CLASS *****/
/* Constructor for the PhysicalDevice class. */
PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_physical_device, uint32_t index) :
    vk_physical_device(vk_physical_device),
    _index(index)
{
    // Query the device for its properties
    this->vk_physical_device_properties = new VkPhysicalDeviceProperties{};
    vkGetPhysicalDeviceProperties(this->vk_physical_device, this->vk_physical_device_properties);

    // Select the proper type
    switch(this->vk_physical_device_properties->deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            this->_type = PhysicalDeviceType::cpu;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            this->_type = PhysicalDeviceType::integrated;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            this->_type = PhysicalDeviceType::discrete;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            this->_type = PhysicalDeviceType::simulated;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            this->_type = PhysicalDeviceType::other;
            break;
        
        default:
            this->_type = PhysicalDeviceType::undefined;

    }
}

/* Copy constructor for the PhysicalDevice class. */
PhysicalDevice::PhysicalDevice(const PhysicalDevice& other) :
    vk_physical_device(other.vk_physical_device),
    _index(other._index),
    _type(other._type)
{
    // Also copy the properties struct
    this->vk_physical_device_properties = new VkPhysicalDeviceProperties(*other.vk_physical_device_properties);
}

/* Move constructor for the PhysicalDevice class. */
PhysicalDevice::PhysicalDevice(PhysicalDevice&& other) :
    vk_physical_device(other.vk_physical_device),
    vk_physical_device_properties(other.vk_physical_device_properties),
    _index(other._index),
    _type(other._type)
{
    other.vk_physical_device_properties = nullptr;
}

/* Destructor for the PhysicalDevice class. */
PhysicalDevice::~PhysicalDevice() {
    if (this->vk_physical_device_properties != nullptr) {
        delete this->vk_physical_device_properties;
    }
}



/* Static function that determines if the given physical device is supported. */
bool PhysicalDevice::is_suitable(VkPhysicalDevice vk_physical_device, VkSurfaceKHR vk_surface, const Tools::Array<const char*>& vk_device_extensions, const Tools::Array<Vulkanic::DeviceFeature>& vk_device_features) {
    // First, check if the device supports all desired queues
    // Get a list of all queue families
    uint32_t n_families;
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &n_families, nullptr);
    Tools::Array<VkQueueFamilyProperties> families(n_families);
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &n_families, families.wdata(n_families));

    // Convert each of those to a QueueFamilyInfo struct
    bool operations_supported[] = { false, false, false, false };
    for (uint32_t i = 0; i < n_families; i++) {
        // Check whether this queue can present
        VkBool32 can_present;
        vkGetPhysicalDeviceSurfaceSupportKHR(vk_physical_device, i, vk_surface, &can_present);

        // Compute the flags for this queue type
        operations_supported[0] |= !!(families[i].queueFlags & VK_QUEUE_TRANSFER_BIT);
        operations_supported[1] |= !!(families[i].queueFlags & VK_QUEUE_COMPUTE_BIT);
        operations_supported[2] |= !!(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT);
        operations_supported[3] |= can_present;
    }

    // If we're missing an operation type, this device is not suitable
    if (!operations_supported[0] || !operations_supported[1] || !operations_supported[2] || !operations_supported[3]) { return false; }

    // Otherwise, make sure it supports the required extensions as well
    if (!gpu_supports_extensions(vk_physical_device, vk_device_extensions)) { return false; }

    // Finally, make sure the required features are supported too
    if (!gpu_supports_features(vk_physical_device, vk_device_features)) { return false; }

    // It it is a suitable GPU!
    return true;
}



/* Swap operator for the PhysicalDevice class. */
void GPU::swap(PhysicalDevice& pd1, PhysicalDevice& pd2) {
    using std::swap;

    swap(pd1.vk_physical_device, pd2.vk_physical_device);
    swap(pd1.vk_physical_device_properties, pd2.vk_physical_device_properties);

    swap(pd1._index, pd2._index);
    swap(pd1._type, pd2._type);
}
