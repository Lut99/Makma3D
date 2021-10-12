/* HARDWARE GPU.cpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:10:33
 * Last edited:
 *   07/10/2021, 22:10:33
 * Auto updated?
 *   Yes
 *
 * Description:
 *   COntains the HardwareGPU class, which wraps around a Vulkan physical
 *   device. Mostly does so to list its properties, but also to aid in
 *   selection of the devices.
**/

#include <cstring>
#include <algorithm>
#include "vulkanic/gpu/HardwareGPU.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Vulkanic;


/***** HARDWAREGPU CLASS *****/
/* Constructor for the HardwareGPU class, which takes the VkPhysicalDevice it is supposed to wrap. */
HardwareGPU::HardwareGPU(const VkPhysicalDevice& vk_physical_device) :
    vk_physical_device(vk_physical_device)
{
    // Query the device for its properties
    this->vk_physical_device_properties = new VkPhysicalDeviceProperties{};
    vkGetPhysicalDeviceProperties(this->vk_physical_device, this->vk_physical_device_properties);

    // Select the proper type
    switch(this->vk_physical_device_properties->deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            this->_type = HardwareGPUType::cpu;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            this->_type = HardwareGPUType::integrated;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            this->_type = HardwareGPUType::discrete;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            this->_type = HardwareGPUType::simulated;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            this->_type = HardwareGPUType::other;
            break;
        
        default:
            this->_type = HardwareGPUType::undefined;

    }
}

/* Copy constructor for the HardwareGPU class. */
HardwareGPU::HardwareGPU(const HardwareGPU& other) :
    vk_physical_device(other.vk_physical_device)
{
    // Also copy the properties
    this->vk_physical_device_properties = new VkPhysicalDeviceProperties{};
    memcpy((void*) this->vk_physical_device_properties, (void*) other.vk_physical_device_properties, sizeof(VkPhysicalDeviceProperties));
}

/* Move constructor for the HardwareGPU class. */
HardwareGPU::HardwareGPU(HardwareGPU&& other) :
    vk_physical_device(other.vk_physical_device),
    vk_physical_device_properties(other.vk_physical_device_properties)
{
    other.vk_physical_device_properties = nullptr;
}

/* Destructor for the HardwareGPU class. */
HardwareGPU::~HardwareGPU() {
    if (this->vk_physical_device_properties != nullptr) {
        delete this->vk_physical_device_properties;
    }
}



/* Returns a QueueFamilyInfo struct with information on how the queue families are laid out for this GPU. The index of each queue family info in the array is the index of the queue family that we're talking about. */
Tools::Array<QueueFamilyInfo> HardwareGPU::get_queue_family_info(const Vulkanic::Surface& surface) const {
    // Get a list of all queue families
    uint32_t n_families;
    vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_device, &n_families, nullptr);
    Tools::Array<VkQueueFamilyProperties> families(n_families);
    vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_device, &n_families, families.wdata(n_families));

    // Convert each of those to a QueueFamilyInfo struct
    Tools::Array<QueueFamilyInfo> result(n_families);
    for (uint32_t i = 0; i < n_families; i++) {
        // Check whether this queue can present
        VkBool32 can_present;
        vkGetPhysicalDeviceSurfaceSupportKHR(this->vk_physical_device, i, surface, &can_present);

        // Compute the flags for this queue type
        QueueTypeFlags queue_types = QueueTypeFlags::none;
        if (families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) { queue_types |= QueueTypeFlags::memory; }
        if (families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)  { queue_types |= QueueTypeFlags::compute; }
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { queue_types |= QueueTypeFlags::graphics; }
        if (can_present)                                    { queue_types |= QueueTypeFlags::present; }

        // Push the new info
        result.push_back(QueueFamilyInfo{
            i,
            queue_types, families[i].queueCount
        });
    }
}



/* Swap operator for the HardwareGPU class. */
void Vulkanic::swap(HardwareGPU& hg1, HardwareGPU& hg2) {
    using std::swap;

    swap(hg1.vk_physical_device, hg2.vk_physical_device);
    swap(hg1.vk_physical_device_properties, hg2.vk_physical_device_properties);
}
