/* GPU.cpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:01:13
 * Last edited:
 *   07/10/2021, 22:01:13
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the GPU class, which wraps around one or more GPUs, and uses
 *   them for rendering.
**/

#include <unordered_set>
#include <algorithm>

#include "tools/Logger.hpp"
#include "arrays/StackArray.hpp"

#include "vulkanic/auxillary/ErrorCodes.hpp"
#include "vulkanic/gpu/GPU.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Vulkanic;


/***** HELPER FUNCTIONS *****/






/***** POPULATE FUNCTIONS *****/
/* Populates a list of VkDeviceQueueCreateInfo structs based on the given queue info of a device. */
static void populate_queue_info(VkDeviceQueueCreateInfo& queue_info, uint32_t family_index, uint32_t n_queues, const Tools::Array<float>& queue_priorities) {
    // Set the meta properties of the struct
    queue_info = {};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    // Next, tell the struct that we want one queue per family
    queue_info.queueFamilyIndex = family_index;
    queue_info.queueCount = n_queues;

    // Finally, give each queue the same priority
    queue_info.pQueuePriorities = queue_priorities.rdata();
}

/* Populates a VkPhysicalDeviceFeatures struct with hardcoded settings. */
static void populate_device_features(VkPhysicalDeviceFeatures& device_features, VkBool32 enable_anisotropy) {
    // None!
    device_features = {};

    // Enable anisotropy if asked to do so
    device_features.samplerAnisotropy = enable_anisotropy;
}

/* Populates a VkDeviceCreateInfo struct based on the given list of qeueu infos and the given device features. */
static void populate_device_info(VkDeviceCreateInfo& device_info, const Tools::Array<VkDeviceQueueCreateInfo>& queue_infos, const VkPhysicalDeviceFeatures& device_features, const Tools::Array<const char*>& device_extensions) {
    // Set the meta info first
    device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Next, pass the queue infos
    device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
    device_info.pQueueCreateInfos = queue_infos.rdata();

    // Pass the device features
    device_info.pEnabledFeatures = &device_features;

    // Finally, add the extensions we want to enable to the device
    device_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    device_info.ppEnabledExtensionNames = device_extensions.rdata();
}





/***** GPU CLASS *****/
/* Constructor for the GPU class. */
GPU::GPU(const Vulkanic::Instance& instance, const Vulkanic::HardwareGPU& physical_device, const QueueFamilyMapping& queue_family_map, GPUFeatureFlags feature_flags, const Tools::Array<const char*>& device_extensions) :
    instance(instance),

    physical_device(physical_device),

    feature_flags(feature_flags),
    vk_extensions(device_extensions)
{
    VkResult vk_result;

    // Prepare creating queue infos
    Tools::Array<float> priorities(1.0f, std::max({ queue_family_map.memory().n_queues, queue_family_map.compute().n_queues, queue_family_map.graphics().n_queues, queue_family_map.present().n_queues }));
    std::unordered_set<uint32_t> unique_queues;

    // Create a VkDeviceQueueCreateInfo struct for each of the (unique) queues
    Tools::Array<VkDeviceQueueCreateInfo> queue_infos(Vulkanic::n_queue_family_types);
    for (size_t i = 0; i < Vulkanic::n_queue_family_types; i++) {
        // Skip if not unique
        if (unique_queues.find(queue_family_map.families[i].index) != unique_queues.end()) { continue; }

        // Otherwise, create the vulkan struct for it
        VkDeviceQueueCreateInfo queue_info;
        populate_queue_info(queue_info, queue_family_map.families[i].index, queue_family_map.families[i].n_queues, priorities);

        // Store it in the list of queue infos and its index in the unique list
        queue_infos.push_back(queue_info);
        unique_queues.insert(queue_family_map.families[i].index);
    }

    // Next, create the device features
    VkPhysicalDeviceFeatures device_features;
    populate_device_features(device_features, (VkBool32) (feature_flags & GPUFeatureFlags::anisotropy));

    // Create the device create info itself
    VkDeviceCreateInfo device_info;
    populate_device_info(device_info, queue_infos, device_features, this->vk_extensions);

    // Create the device!
    if ((vk_result = vkCreateDevice(this->physical_device, &device_info, nullptr, &this->vk_device)) != VK_SUCCESS) {
        logger.fatalc(GPU::channel, "Could not create the logical device: ", vk_error_map[vk_result]);
    }



    // Before we quit, use the device to generate the queue references
    for (uint32_t i = 0; i < Vulkanic::n_queue_family_types; i++) {
        // First, map the index
        this->queue_index_map.insert({ Vulkanic::queue_family_types[i], queue_family_map.families[i].index });

        // Insert a new array for this type & populate it
        this->queue_map.insert({ Vulkanic::queue_family_types[i], Tools::Array<VkQueue>((VkQueue) nullptr, queue_family_map.families[i].n_queues) });
        Tools::Array<VkQueue>& result = this->queue_map.at(Vulkanic::queue_family_types[i]);
        for (uint32_t j = 0; j < queue_family_map.families[i].n_queues; j++) {
            vkGetDeviceQueue(this->vk_device, queue_family_map.families[i].index, j, &result[j]);
        }
    }
}

/* Move constructor for the GPU class. */
GPU::GPU(GPU&& other) :
    instance(other.instance),

    physical_device(std::move(other.physical_device)),

    vk_device(other.vk_device),
    feature_flags(other.feature_flags),
    vk_extensions(std::move(other.vk_extensions)),

    queue_index_map(other.queue_index_map),
    queue_map(other.queue_map)
{
    other.vk_device = nullptr;
}

/* Destructor for the GPU class. */
GPU::~GPU() {
    if (this->vk_device != nullptr) {
        vkDestroyDevice(this->vk_device, nullptr);
    }
}



/* Given a hardware device and a target surface, returns a possible queue family mapping for each type of operation that the engine will do. */
QueueFamilyMapping GPU::create_standard_mapping(const HardwareGPU& physical_device, const Surface& surface) {
    // Prepare the result struct
    QueueFamilyMapping result{};
    // Get a list of the required QueueInfo's
    Tools::Array<QueueFamilyInfo> family_infos = physical_device.get_queue_family_info(surface);

    // Next, loop through all the family infos to count how many capabilities they have
    Tools::Array<uint32_t> capabilities_count(0U, family_infos.size());
    for (uint32_t i = 0; i < family_infos.size(); i++) {
        for (uint32_t j = 0; j < Vulkanic::n_queue_family_types; j++) {
            capabilities_count[i] += !!(family_infos[i].type & Vulkanic::queue_family_types[j]);
        }
    }

    // Finally, loop through all types to find a queue family for them
    Tools::Array<uint32_t> used_count(0U, family_infos.size());
    for (uint32_t i = 0; i < Vulkanic::n_queue_family_types; i++) {
        // Loop through the queues to find the best one
        uint32_t best_used_count = std::numeric_limits<uint32_t>::max(), best_capability_count = std::numeric_limits<uint32_t>::max(), best_queue_family;
        for (uint32_t j = 0; j < family_infos.size(); j++) {
            // If it doesn't support this capability, then we're immediately done with it
            const QueueFamilyInfo& family = family_infos[j];
            if (!(family.type & Vulkanic::queue_family_types[i])) { continue; }

            // Otherwise, don't used this queue if it's used more than the current queue
            if (used_count[j] > best_used_count) { continue; }
            // Next, skip if this queue has more (or the same) capabilities than the best one
            if (capabilities_count[j] >= best_capability_count) { continue; }

            // Otherwise, it's better, so store it!
            best_used_count = used_count[j];
            best_capability_count = capabilities_count[j];
            best_queue_family = j;
        }

        // If the best used count is still 'infinite', then we didn't find any that supported this operation
        if (best_used_count == std::numeric_limits<uint32_t>::max()) {
            logger.fatalc(GPU::channel, "Physical device '", physical_device.name(), "' doesn't support ", queue_type_flags_names.at(Vulkanic::queue_family_types[i]), "-operations; cannot map it.");
        }

        // Mark the selected queue as the actually used one
        ++used_count[best_queue_family];
        result.families[i] = family_infos[best_queue_family];
    }

    // Done!
    return result;
}



/* Returns whether or not the given extension is enabled in this device. */
bool GPU::has_extension(const char* extension) const {
    for (uint32_t i = 0; i < this->vk_extensions.size(); i++) {
        if (strcmp(this->vk_extensions[i], extension) == 0) { return true; }
    }
    return false;
}



/* Swap operator for the GPU class. */
void Vulkanic::swap(GPU& g1, GPU& g2) {
    #ifndef NDEBUG
    if (&g1.instance != &g2.instance) { logger.fatalc(GPU::channel, "Cannot swap GPUs with different instances."); }
    #endif

    using std::swap;

    swap(g1.physical_device, g2.physical_device);
    
    swap(g1.vk_device, g2.vk_device);
    swap(g1.feature_flags, g2.feature_flags);
    swap(g1.vk_extensions, g2.vk_extensions);
    
    swap(g1.queue_index_map, g2.queue_index_map);
    swap(g1.queue_map, g2.queue_map);
}
