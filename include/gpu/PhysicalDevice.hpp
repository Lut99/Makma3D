/* PHYSICAL DEVICE.hpp
 *   by Lut99
 *
 * Created:
 *   13/10/2021, 16:38:20
 * Last edited:
 *   13/10/2021, 16:38:20
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the PhysicalDevice class, which represents a single,
 *   supported GPU that is registered in the Vulkan instance.
**/

#ifndef GPU_PHYSICAL_DEVICE_HPP
#define GPU_PHYSICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "arrays/Array.hpp"

#include "vulkanic/gpu/DeviceFeature.hpp"

#include "PhysicalDeviceType.hpp"

namespace Makma3D {
    /* The PhysicalDevice class, which represents a single GPU registered in the Vulkan instance. */
    class PhysicalDevice {
    public:
        /* Channel name for the PhysicalDevice class. */
        static constexpr const char* channel = "PhysicalDevice";

    private:
        /* The VkPhysicalDevice we wrap. */
        VkPhysicalDevice vk_physical_device;
        /* The properties of the physical device. */
        VkPhysicalDeviceProperties* vk_physical_device_properties;

        /* The index of the physical device in the list. */
        uint32_t _index;
        /* The HardwareGPU type of the GPU. */
        PhysicalDeviceType _type;

    public:
        /* Constructor for the PhysicalDevice class.
         * @param vk_physical_device The Vulkan physical device we wrap. Will be automatically deallocated when this class is.
         * @param index The index of the physical device in the list of devices. */
        PhysicalDevice(VkPhysicalDevice vk_physical_device, uint32_t index);
        /* Copy constructor for the PhysicalDevice class. */
        PhysicalDevice(const PhysicalDevice& other);
        /* Move constructor for the PhysicalDevice class. */
        PhysicalDevice(PhysicalDevice&& other);
        /* Destructor for the PhysicalDevice class. */
        ~PhysicalDevice();

        /* Static function that determines if the given physical device is supported.
         * @param vk_physical_device The Vulkan physical device that we're gonna check out.
         * @param vk_surface The VkSurface object used to check if this device can present to that Surface.
         * @param vk_device_extensions The list of Vulkan GPU extensions that the device should at least support.
         * @param vk_device_features The list of Vulkan GPU features (as Vulkanic::DeviceFeatures enums) that the device should at least support.
         * @returns Whether or not this device is suitable for the Makma3D engine (true) or not (false). */
        static bool is_suitable(VkPhysicalDevice vk_physical_device, VkSurfaceKHR vk_surface, const Tools::Array<const char*>& vk_device_extensions, const Tools::Array<Vulkanic::DeviceFeature>& vk_device_features);

        /* Returns the index of the GPU in Vulkan's list. */
        inline uint32_t index() const { return this->_index; }
        /* Returns the name of the GPU. */
        inline const char* name() const { return this->vk_physical_device_properties->deviceName; }
        /* Returns the Vulkan-assigned type of the GPU. */
        inline PhysicalDeviceType type() const { return this->_type; }
        /* Explicitly returns the internal VkPhysicalDevice object. */
        inline const VkPhysicalDevice& vk() const { return this->vk_physical_device; }
        /* Implicitly returns the internal VkPhysicalDevice object. */
        inline operator const VkPhysicalDevice&() const { return this->vk_physical_device; }

        /* Copy assignment operator for the PhysicalDevice class. */
        inline PhysicalDevice& operator=(const PhysicalDevice& other) { return *this = PhysicalDevice(other); }
        /* Move assignment operator for the PhysicalDevice class. */
        inline PhysicalDevice& operator=(PhysicalDevice&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the PhysicalDevice class. */
        friend void swap(PhysicalDevice& pd1, PhysicalDevice& pd2);

    };

    /* Swap operator for the PhysicalDevice class. */
    void swap(PhysicalDevice& pd1, PhysicalDevice& pd2);

}

#endif
