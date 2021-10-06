/* INSTANCE.hpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 11:19:00
 * Last edited:
 *   06/10/2021, 11:19:00
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class, which functions as the application-wide
 *   instance of the Makma3D library.
**/

#ifndef INSTANCE_INSTANCE_HPP
#define INSTANCE_INSTANCE_HPP

#include <string>

#include "arrays/Array.hpp"
#include "glfw/Instance.hpp"
#include "vulkanic/instance/Instance.hpp"

namespace Makma3D {
    /* The Instance class, which is the application-wide instance of the Makma3D library. */
    class Instance {
    public:
        /* Channel name for the Instance class. */
        static constexpr const char* channel = "Instance";

    private:
        /* The GLFW instance that we need. */
        GLFW::Instance glfw_instance;
        /* The Vulkan instance that we need. */
        Vulkanic::Instance vulkan_instance;

    public:
        /* Constructor for the Instance class, which takes the application name, the application version (created with VK_MAKE_VERSION), a list of Vulkan extensions to enable and a list of Vulkan layers to enable. If NDEBUG isn't defined, the Vulkan debug extension & layers are automatically enabled. */
        Instance(const std::string& application_name, uint32_t application_version, const Tools::Array<const char*>& vulkan_extensions, const Tools::Array<const char*>& vulkan_layers);
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Explicitly returns the VkInstance class of the internal Vulkan Instance. */
        inline const VkInstance& vk() const { return this->vulkan_instance.vk(); }
        /* Implicitly returns the VkInstance class of the internal Vulkan Instance. */
        inline operator const VkInstance&() const { return this->vulkan_instance.vk(); }

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
