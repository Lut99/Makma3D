/* HARDWARE GPU.hpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:10:29
 * Last edited:
 *   07/10/2021, 22:10:29
 * Auto updated?
 *   Yes
 *
 * Description:
 *   COntains the HardwareGPU class, which wraps around a Vulkan physical
 *   device. Mostly does so to list its properties, but also to aid in
 *   selection of the devices.
**/

#ifndef VULKANIC_HARDWARE_GPU_HPP
#define VULKANIC_HARDWARE_GPU_HPP

#include <vulkan/vulkan.h>

namespace Makma3D::Vulkanic {
    /* The HardwareGPU class, which wraps a single physical device and lists its properties. */
    class HardwareGPU {
    private:
        /* The VkPhysicalDevice we wrap. */
        VkPhysicalDevice vk_physical_device;
        /* The properties of the physical device. */
        VkPhysicalDeviceProperties* vk_physical_device_properties;
    
    public:
        /* Constructor for the HardwareGPU class, which takes the VkPhysicalDevice it is supposed to wrap. */
        HardwareGPU(const VkPhysicalDevice& vk_physical_device);
        /* Copy constructor for the HardwareGPU class. */
        HardwareGPU(const HardwareGPU& other);
        /* Move constructor for the HardwareGPU class. */
        HardwareGPU(HardwareGPU&& other);
        /* Destructor for the HardwareGPU class. */
        ~HardwareGPU();

        /* Returns the name of the GPU. */
        inline const char* name() const { return this->vk_physical_device_properties->deviceName; }
        /* Returns the Vulkan-assigned type of the GPU. */
        inline VkPhysicalDeviceType type() const { return this->vk_physical_device_properties->deviceType; }

        /* Explicitly returns the internal VkPhysicalDevice object. */
        inline const VkPhysicalDevice& vk() const { return this->vk_physical_device; }
        /* Implicitly returns the internal VkPhysicalDevice object. */
        inline operator const VkPhysicalDevice&() const { return this->vk_physical_device; }

        /* Copy assignment operator for the HardwareGPU class. */
        inline HardwareGPU& operator=(const HardwareGPU& other) { return *this = HardwareGPU(other); }
        /* Move assignment operator for the HardwareGPU class. */
        inline HardwareGPU& operator=(HardwareGPU&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the HardwareGPU class. */
        friend void swap(HardwareGPU& hg1, HardwareGPU& hg2);

    };
    
    /* Swap operator for the HardwareGPU class. */
    void swap(HardwareGPU& hg1, HardwareGPU& hg2);

}

#endif
