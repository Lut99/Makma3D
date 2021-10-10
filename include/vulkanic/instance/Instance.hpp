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
#include "vulkanic/gpu/HardwareGPU.hpp"

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

        /* The list of physical devices registered to this instance. */
        Tools::Array<Vulkanic::HardwareGPU> _physical_devices;

        /* The extensions used to create the instance. */
        Tools::Array<const char*> vk_extensions;
        /* The layers used to create the instance. */
        Tools::Array<const char*> vk_layers;
    
    public:
        /* Constructor for the Instance class, which takes the application name, the application version, a list of extensions to enable at the instance and layers to enable. Create the version with VK_MAKE_VERSION. */
        Instance(const std::string& application_name, uint32_t application_version, const Tools::Array<const char*>& extensions, const Tools::Array<const char*>& layers);
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Returns the list of HardwareGPUs registered to the Instance. */
        inline const Tools::Array<Vulkanic::HardwareGPU>& physical_devices() const { return this->_physical_devices; }

        /* Returns whether or not the given extension is enabled in this Instance. */
        inline bool has_extension(const std::string& extension) const { return this->has_extension(extension.c_str()); }
        /* Returns whether or not the given extension is enabled in this Instance. */
        bool has_extension(const char* extension) const;
        /* Returns whether or not the given layer is enabled in this Instance. */
        inline bool has_layer(const std::string& layer) const { return this->has_layer(layer.c_str()); }
        /* Returns whether or not the given layer is enabled in this Instance. */
        bool has_layer(const char* layer) const;
        /* Returns the list of extensions with which the Instance was created. */
        inline const Tools::Array<const char*>& extensions() const { return this->vk_extensions; }
        /* Returns the list of layers with which the Instance was created. */
        inline const Tools::Array<const char*>& layers() const { return this->vk_layers; }

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
