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
#include <unordered_set>

#include "arrays/Array.hpp"
#include "gpu/DeviceFeature.hpp"
#include "window/Instance.hpp"
#include "vulkanic/instance/Instance.hpp"

#include "Version.hpp"
#include "Extension.hpp"

namespace Makma3D {
    /* The Instance class, which is the application-wide instance of the Makma3D library. */
    class Instance {
    public:
        /* Channel name for the Instance class. */
        static constexpr const char* channel = "Instance";
        /* The version of the Makma3D engine. */
        static const Version version;
    
    private:
        /* The enabled Makma3D extensions in this Instance. */
        std::unordered_set<Extension> extensions;
        /* The Window instance that handles the GLFW side of instancing. */
        GLFW::Instance glfw_instance;
        /* The Vulkan instance that handles the Vulkan side of instancing. */
        Vulkanic::Instance vk_instance;


        /* Declare the Window class a friend of ours. */
        friend class Window;

    public:
        /* Protected constructor for the Instance class.
         * @param application_name Name for your application. Could be used by Vulkan for optimisation, now or when your application becomes insanely popular.
         * @param application_version Version number of your application. Could be used by Vulkan for optimisation, now or when your application becomes insanely popular.
         * @param extensions List of extensions to enable. Not enabling them can cause certain Makma3D-functions to fail, but try to enable as few as possible for optimisation purposes.
         */
        Instance(const std::string& application_name, const Version& application_version, const Tools::Array<Extension>& extensions);
        /* Copy constructor for the Instance class, which is deleted. */
        Instance(const Instance& other) = delete;
        /* Move constructor for the Instance class. */
        Instance(Instance&& other);
        /* Destructor for the Instance class. */
        ~Instance();

        /* Checks whether the given Extension is enabled in this instance.
         * @param ext The extension you wish to check if it's enabled.
         */
        inline bool extension_enabled(Extension ext) const { return this->extensions.find(ext) != this->extensions.end(); }
        /* Returns a list of enabled Extensions that can be iterated through. */
        Tools::Array<Extension> get_extensions() const;
        /* Returns a list of Vulkan device extensions, based on the enabled Makma3D extensions + the ones we always require. */
        Tools::Array<const char*> get_device_extensions() const;
        /* Returns a list of Vulkan device featyres, based on the enabled Makma3D extensions + the ones we always require. */
        Tools::Array<Vulkanic::DeviceFeature> get_device_features() const;

        /* Returns the primary monitor as given by GLFW. */
        inline const Monitor* get_primary_monitor() const { return this->glfw_instance.get_primary_monitor(); }
        /* Returns the list of available monitors as given by GLFW. */
        inline const Tools::Array<const Monitor*>& get_monitors() const { return this->glfw_instance.get_monitors(); }

        /* Explicitly returns the internal VkInstance object. */
        inline const VkInstance& vk() const { return this->vk_instance.vk(); }
        /* Implicitly returns the internal VkInstance object. */
        inline operator const VkInstance&() const { return this->vk_instance.vk(); }

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
