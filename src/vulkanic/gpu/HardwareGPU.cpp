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



/* Swap operator for the HardwareGPU class. */
void Vulkanic::swap(HardwareGPU& hg1, HardwareGPU& hg2) {
    using std::swap;

    swap(hg1.vk_physical_device, hg2.vk_physical_device);
    swap(hg1.vk_physical_device_properties, hg2.vk_physical_device_properties);
}
