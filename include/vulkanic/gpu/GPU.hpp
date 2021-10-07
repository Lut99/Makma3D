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

#include <vulkan/vulkan.h>

#include "vulkanic/instance/Instance.hpp"

#include "GPUFeatureFlags.hpp"
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

    public:
        /* Constructor for the GPU class, which takes a Vulkan Instance to create the GPU with, a HardwareGPU to wrap around and flags that determine which GPU features are enabled. */
        GPU(const Vulkanic::Instance& instance, const Vulkanic::HardwareGPU& physical_device, GPUFeatureFlags feature_flags);
        /* Copy constructor for the GPU class, which is deleted. */
        GPU(const GPU& other) = delete;
        /* Move constructor for the GPU class. */
        GPU(GPU&& other);
        /* Destructor for the GPU class. */
        ~GPU();

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
