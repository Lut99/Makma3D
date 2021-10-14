/* DEVICE.hpp
 *   by Lut99
 *
 * Created:
 *   14/10/2021, 11:16:36
 * Last edited:
 *   14/10/2021, 11:16:36
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Device class, which is build around a PhysicalDevice to
 *   create a conceptual Device with much more functionality.
**/

#ifndef GPU_DEVICE_HPP
#define GPU_DEVICE_HPP

#include <vulkan/vulkan.h>

#include "instance/Instance.hpp"

#include "QueueType.hpp"
#include "PhysicalDevice.hpp"

namespace Makma3D {
    /* The Device class, which wraps around a PhysicalDevice to create an instantiated conceptual version of a GPU. */
    class Device {
    public:
        /* Logging channel name for the Device class. */
        static constexpr const char* channel = "Device";

        /* The Instance around which this Device is build. */
        const Makma3D::Instance& instance;

    private:
        /* The PhysicalDevice around which we wrap. */
        PhysicalDevice physical_device;

        /* The Vulkan VkDevice object. */
        VkDevice vk_device;
        /* Lists the queues for each QueueType. */
        Tools::Array<Tools::Array<VkQueue>> queues;

    public:
        /* Constructor for the Device class.
         * @param instance The Makma3D instance where we will allocate the GPU.
         * @param physical_device The PhysicalDevice object which we want to wrap around/instantiate.
         * @param vk_surface The VkSurfaceKHR object used to map the physical device's queues to type of operations we want to perform (and specifically, to determine present capabilities of a queue. */
        Device(const Makma3D::Instance& instance, const PhysicalDevice& physical_device, VkSurfaceKHR vk_surface);
        /* Copy constructor for the Device class, which is deleted. */
        Device(const Device& other) = delete;
        /* Move constructor for the Device class. */
        Device(Device&& other);
        /* Destructor for the Device class. */
        ~Device();

        /* Returns how many queues are available of the given type.
         * @param queue_type The type of operation for which we want to check the number of queues.
         * @returns The number of queues used for the given operation type. */
        inline uint32_t get_queue_count(Vulkanic::QueueType queue_type) const { return this->queues[(uint32_t) queue_type].size(); }
        /* Returns the i'th queue of the given queue type. There's guaranteed to be at least one.
         * @param queue_type The type of operation of the queue list where we want to get a queue from.
         * @param index The index of the desired queue in the list of queues.
         * @returns The desired queue as a VkQueue object. */
        inline const VkQueue& get_queue(Vulkanic::QueueType queue_type, uint32_t index) const { return this->queues[(uint32_t) queue_type][index]; }

        /* Explicitly returns the internal VkDevice object. */
        inline const VkDevice& vk() const { return this->vk_device; }
        /* Implicitly returns the internal VkDevice object. */
        inline operator const VkDevice&() const { return this->vk_device; }

        /* Copy assignment operator for the Device class, which is deleted. */
        Device& operator=(const Device& other) = delete;
        /* Move assignment operator for the Device class, which is deleted. */
        inline Device& operator=(Device&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the Device class. */
        friend void swap(Device& d1, Device& d2);

    };

    /* Swap operator for the Device class. */
    void swap(Device& d1, Device& d2);

}

#endif
