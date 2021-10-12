/* GPU.hpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:01:16
 * Last edited:
 *   07/10/2021, 22:01:16
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the GPU class, which wraps around one (and for now only one)
 *   GPU, and uses it for rendering.
**/

#ifndef VULKANIC_GPU_HPP
#define VULKANIC_GPU_HPP

#include <unordered_map>
#include <vulkan/vulkan.h>

#include "arrays/Array.hpp"
#include "vulkanic/instance/Instance.hpp"

#include "GPUFeatureFlags.hpp"
#include "QueueFamilyMapping.hpp"
#include "HardwareGPU.hpp"

namespace Makma3D::Vulkanic {
    /* The GPU class, which is used to represent one physical device to render to. */
    class GPU {
    public:
        /* Channel name for the GPU class. */
        static constexpr const char* channel = "VulkanicGPU";

        /* The Instance with which this GPU has been created. */
        const Vulkanic::Instance& instance;
    
    private:
        /* The GPU around which we wrap. */
        Vulkanic::HardwareGPU physical_device;
        
        /* The logical device around which we wrap. */
        VkDevice vk_device;
        /* The flags we enabled for this device. */
        GPUFeatureFlags feature_flags;
        /* The extensions we enabled for this device. */
        Tools::Array<const char*> vk_extensions;

        /* Map of queue types to the chosen family indices. */
        std::unordered_map<QueueTypeFlags, uint32_t> queue_index_map;
        /* Map of queue types to the list of queues available for that type. */
        std::unordered_map<QueueTypeFlags, Tools::Array<VkQueue>> queue_map;

    public:
        /* Constructor for the GPU class.
         * @param instance The Vulkanic instance with which we can initialize the GPU.
         * @param physical_device The physical device around which we will wrap this GPU.
         * @param queue_family_map A mapping from a queue capability to the queue family index the GPU should use for it. If you have no preference about how to do this, see the GPU's static 'create_standard_mapping()' function.
         * @param feature_flags Flags determining which GPU features to enable.
         * @param device_extensions List of extensions to enable on this GPU instance. The Makma library might set some extensions itself if it needs them.
         */
        GPU(const Vulkanic::Instance& instance, const Vulkanic::HardwareGPU& physical_device, const QueueFamilyMapping& queue_family_map, GPUFeatureFlags feature_flags, const Tools::Array<const char*>& device_extensions);
        /* Copy constructor for the GPU class, which is deleted. */
        GPU(const GPU& other) = delete;
        /* Move constructor for the GPU class. */
        GPU(GPU&& other);
        /* Destructor for the GPU class. */
        ~GPU();

        /* Given a hardware device and a target surface, returns a possible queue family mapping for each type of operation that the engine will do. 
         * Tries to optimise by trying to select as many different queue families as possible.
         * @param physical_device The physical device to create the mapping for.
         * @param surface The surface used to determine if a queue family can present to that surface.
         */
        static QueueFamilyMapping create_standard_mapping(const HardwareGPU& physical_device, const Surface& surface);

        /* Returns whether or not the given extension is enabled in this device.
         * @param extension Name of the extension that should be checked.
         */
        inline bool has_extension(const std::string& extension) const { return this->has_extension(extension.c_str()); }
        /* Returns whether or not the given extension is enabled in this device.
         * @param extension Name of the extension that should be checked.
         */
        bool has_extension(const char* extension) const;
        /* Returns the list of extensions with which the device was created. */
        inline const Tools::Array<const char*>& extensions() const { return this->vk_extensions; }

        /* Returns the family index for the given queue type. */
        inline uint32_t get_queue_index(QueueTypeFlags queue_type) const { return this->queue_index_map.at(queue_type); };
        /* Returns the first queue of the given type. */
        inline const VkQueue& get_queue(QueueTypeFlags queue_type) const { return this->queue_map.at(queue_type).first(); }

        /* Explicitly returns the internal VkDevice object. */
        inline const VkDevice& vk() const { return this->vk_device; }
        /* Implicitly returns the internal VkDevice object. */
        inline operator const VkDevice&() const { return this->vk_device; }

        /* Copy assignment operator for the GPU class, which is deleted. */
        GPU& operator=(const GPU& other) = delete;
        /* Move assignment operator for the GPU class. */
        inline GPU& operator=(GPU&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the GPU class. */
        friend void swap(GPU& g1, GPU& g2);

    };
    
    /* Swap operator for the GPU class. */
    void swap(GPU& g1, GPU& g2);

}

#endif
