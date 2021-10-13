/* INSTANCE.hpp
 *   by Lut99
 *
 * Created:
 *   30/04/2021, 14:03:39
 * Last edited:
 *   06/10/2021, 15:36:55
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class, which wraps the Vulkan instance and
 *   manages the debugger.
**/

#ifndef COMPUTE_INSTANCE_HPP
#define COMPUTE_INSTANCE_HPP

#include <vulkan/vulkan.h>

#include "arrays/Array.hpp"
#include "gpu/PhysicalDevice.hpp"

namespace Makma3D::Vulkanic {
    /* The Vulkan instance extensions we want to be enabled. */
    const Tools::Array<const char*> instance_extensions({
        #ifndef NDEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        #endif
    });
    /* The Vulkan validation layers we want to be enabled. */
    const Tools::Array<const char*> debug_layers({
        #ifndef NDEBUG
        "VK_LAYER_KHRONOS_validation" 
        #endif
    });



    /* The Instance class, which wraps and manages the Vulkan instance and the Vulkan debug logger. */
    class Instance {
    public:
        /* The channel used for the Instance class. */
        static constexpr const char* channel = "VulkanicInstance";

    private:
        /* The VkInstance that we use, among other things. */
        VkInstance vk_instance;

        /* The debug messenger of Vulkan. */
        VkDebugUtilsMessengerEXT vk_debugger;
        /* The function needed to destroy the Vulkan debug messenger. */
        PFN_vkDestroyDebugUtilsMessengerEXT vk_destroy_debug_utils_messenger_method;
    
    public:
        /* Constructor for the Instance class.
         * Doesn't do the actual initialization, though; use init() and init_debug() for that. */
        Instance();
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Initializes the instance.
         * @param application_name The name of the application.
         * @param application_version The version of the application, as given by VK_MAKE_VERSION().
         * @param makma_version The version of the Makma3D engine, as given by VK_MAKE_VERSION().
         * @param extensions The list of Vulkan extensions to enable.
         * @param layers The list of Vulkan layers to enable. */
        void init(const char* application_name, uint32_t application_version, uint32_t makma_version, const Tools::Array<const char*>& extensions, const Tools::Array<const char*>& layers);
        /* Initializes the debugging part of the instance.
         * Requires the appropriate extensions and layers already to be defined during the init() stage. */
        void init_debug();

        /* Returns the list of (supported) PhysicalDevices that are currently registered to the Vulkan backend.
         * @param vk_surface The VkSurface object used to check if this device can present to that Surface.
         * @param vk_device_extensions The list of Vulkan GPU extensions that the device should at least support.
         * @param vk_device_features The list of Vulkan GPU features (as Vulkanic::DeviceFeatures enums) that the device should at least support.
         * @returns The list of Makma3D-suitable physical devices that we found. Empty if no such devices are present. */
        Tools::Array<PhysicalDevice> get_physical_devices(VkSurfaceKHR vk_surface, const Tools::Array<const char*>& vk_device_extensions, const Tools::Array<Vulkanic::DeviceFeature>& vk_device_features) const;

        /* Explicitly returns the internal VkInstance object. */
        inline const VkInstance& vk() const { return this->vk_instance; }
        /* Implicitly returns the internal VkInstance object. */
        inline operator const VkInstance&() const { return this->vk_instance; }

        /* Copy assignment operator for the Instance class, which is deleted. */
        Instance& operator=(const Instance& other) = delete;
        /* Move assignment operator for the Instance class. */
        inline Instance& operator=(Instance&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the Instance class. */
        friend void swap(Instance& i1, Instance& i2);

    };

    /* Swap operator for the Instance class. */
    void swap(Instance& i1, Instance& i2);
}

#endif
