/* DEVICE.cpp
 *   by Lut99
 *
 * Created:
 *   14/10/2021, 11:16:39
 * Last edited:
 *   14/10/2021, 11:16:39
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Device class, which is build around a PhysicalDevice to
 *   create a conceptual Device with much more functionality.
**/

#include "tools/Logger.hpp"
#include "arrays/StackArray.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "gpu/Device.hpp"

using namespace std;
using namespace Makma3D;


/***** HELPER FUNCTIONS *****/
/* Maps the different kind of Device operations to queue families. Tries to select families which are least used and have the most specific capabilities.
 * @param physical_device The PhysicalDevice who's queues we'd like to map. 
 * @param vk_surface The VkSurfaceKHR we'll use to check presentability of a queue family.
 * @returns A static array with as elements a pair of the queue family index and the queue count, for each of the queue types / queue operations. */
static Tools::StackArray<std::pair<uint32_t, uint32_t>, Vulkanic::n_queue_types> map_queue_families(const PhysicalDevice& physical_device, VkSurfaceKHR vk_surface) {
    // Prepare the result list
    Tools::StackArray<std::pair<uint32_t, uint32_t>, Vulkanic::n_queue_types> result;

    // Collect a list of queue families
    uint32_t n_queue_families;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &n_queue_families, nullptr);
    Tools::Array<VkQueueFamilyProperties> queue_families(n_queue_families);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &n_queue_families, queue_families.wdata(n_queue_families));

    // Next, loop through all the family infos to count how many capabilities they have
    Tools::Array<Tools::StackArray<bool, Vulkanic::n_queue_types>> capabilities(Tools::StackArray<bool, Vulkanic::n_queue_types>(false, Vulkanic::n_queue_types), queue_families.size());
    Tools::Array<uint32_t> capabilities_count(0U, queue_families.size());
    for (uint32_t i = 0; i < queue_families.size(); i++) {
        // Check if the queue can present
        VkBool32 can_present;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, vk_surface, &can_present);

        // Count the capabilities, immediately noting this queue's support in the capabilities list
        capabilities_count[i] += (capabilities[i][0] = !!(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT));
        capabilities_count[i] += (capabilities[i][1] = !!(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT));
        capabilities_count[i] += (capabilities[i][2] = !!(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT));
        capabilities_count[i] += (capabilities[i][3] = !!can_present);
    }

    // Finally, loop through all types to find a queue family for them
    Tools::Array<uint32_t> used_count(0U, queue_families.size());
    for (uint32_t i = 0; i < Vulkanic::n_queue_types; i++) {
        // Loop through the queues to find the best one
        uint32_t best_used_count = std::numeric_limits<uint32_t>::max(), best_capability_count = std::numeric_limits<uint32_t>::max(), best_queue_family;
        for (uint32_t j = 0; j < queue_families.size(); j++) {
            // If it doesn't support this capability, then we're immediately done with it
            const VkQueueFamilyProperties& family = queue_families[j];
            if (!capabilities[j][i]) { continue; }

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
            logger.fatalc(Device::channel, "Physical device '", physical_device.name(), "' doesn't support ", Vulkanic::queue_type_names[i], "-operations; cannot map it.");
        }

        // Mark the selected queue as the actually used one
        ++used_count[best_queue_family];
        result.push_back({ best_queue_family, queue_families[best_queue_family].queueCount });
    }

    // Done!
    return result;
}

/* Given a StackArray that is a queue map, returns an equivalent Array that contains only unique entries.
 * @param queue_family_map The StackArray that is the queue map from which we want to extract the unique entries.
 * @returns A new Array that has one <index, count> pair for each unique queue family. */
static Tools::Array<std::pair<uint32_t, uint32_t>> uniqueify_queue_map(const Tools::StackArray<std::pair<uint32_t, uint32_t>, Vulkanic::n_queue_types>& queue_family_map) {
    // Loop through all queue mappings to get their unique count
    Tools::Array<std::pair<uint32_t, uint32_t>> result(queue_family_map.size());
    for (uint32_t i = 0; i < queue_family_map.size(); i++) {
        // Check if we already added this index
        for (uint32_t j = 0; j < result.size(); j++) {
            if (queue_family_map[i].first == result[j].first) { continue; }
        }

        // Otherwise, add it
        result.push_back(queue_family_map[i]);
    }
    // Done, return
    return result;
}





/***** POPULATE FUNCTIONS *****/
/* Populates the given VkDeviceQueueCreateInfo struct.
 * @param queue_info The VkDeviceQueueCreateInfo struct we want to populate.
 * @param queue_index The queue family index of the family we want to initialize.
 * @param queue_count The number of queues we want to initialize of the given family.
 * @param queue_priorities A list of queue priorities, which must be at least queue_count in size. */
static void populate_queue_info(VkDeviceQueueCreateInfo& queue_info, uint32_t queue_index, uint32_t queue_count, const Tools::Array<float>& queue_priorities) {
    // Set the meta properties of the struct
    queue_info = {};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    // Next, tell the struct that we want one queue per family
    queue_info.queueFamilyIndex = queue_index;
    queue_info.queueCount = queue_count;

    // Finally, give each queue the same priority
    queue_info.pQueuePriorities = queue_priorities.rdata();
}

/* Populates the given VkPhysicalDeviceFeatures struct.
 * @param device_features The VkPhysicalDeviceFeatures struct to populate.
 * @param makma_device_features The DeviceFeatures as specified by the Makma library to enable. */
static void populate_device_features(VkPhysicalDeviceFeatures& device_features, const Tools::Array<Vulkanic::DeviceFeature>& makma_device_features) {
    // Initialize the struct to default
    device_features = {};

    // Enable those we want to enable
    for (uint32_t i = 0; i < makma_device_features.size(); i++) {
        switch(makma_device_features[i]) {
        case Vulkanic::DeviceFeature::anisotropy:
            device_features.samplerAnisotropy = VK_TRUE;
            break;

        default:
            logger.warningc(Device::channel, "Unknown Makma3D device feature '", Vulkanic::device_feature_names[(int) makma_device_features[i]], "' encountered; skipping.");

        }
    }

    // Done
}

/* Populates the given VkDeviceCreateInfo struct. 
 * @param device_info The VkDeviceCreateInfo struct to populate.
 * @param queue_infos The list of VkDeviceQueueCreateInfo that specify how many queues to create and from which family.
 * @param device_extensions The list of device extensions to enable for this device.
 * @param device_features The VkPhysicalDeviceFeatures struct listing which features to enable for this device. */
static void populate_device_info(VkDeviceCreateInfo& device_info, const Tools::Array<VkDeviceQueueCreateInfo>& queue_infos, const Tools::Array<const char*>& device_extensions, const VkPhysicalDeviceFeatures& device_features) {
    // Set the meta info first
    device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Next, pass the queue infos
    device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
    device_info.pQueueCreateInfos = queue_infos.rdata();

    // Add the extensions we want to enable to the device
    device_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    device_info.ppEnabledExtensionNames = device_extensions.rdata();

    // Finally, pass the device features
    device_info.pEnabledFeatures = &device_features;
}





/***** DEVICE CLASS *****/
/* Constructor for the Device class. */
Device::Device(const Makma3D::Instance& instance, const PhysicalDevice& physical_device, VkSurfaceKHR vk_surface) :
    instance(instance),

    physical_device(physical_device),
    queues({}, Vulkanic::n_queue_types)
{
    // First, map the queue families
    Tools::StackArray<std::pair<uint32_t, uint32_t>, Vulkanic::n_queue_types> queue_family_map = map_queue_families(physical_device, vk_surface);
    // Extract the list of unique queue families from it
    Tools::Array<std::pair<uint32_t, uint32_t>> unique_queue_family_map = uniqueify_queue_map(queue_family_map);
    // Finally, construct a list of priorities that is large enough for all queue families
    Tools::Array<float> priorities;
    for (uint32_t i = 0; i < unique_queue_family_map.size(); i++) {
        priorities.resize(1.0f, unique_queue_family_map[i].second);
    }

    // Use that to create the queueinfos
    Tools::Array<VkDeviceQueueCreateInfo> queue_infos({}, unique_queue_family_map.size());
    for (uint32_t i = 0; i < unique_queue_family_map.size(); i++) {
        populate_queue_info(queue_infos[i], unique_queue_family_map[i].first, unique_queue_family_map[i].second, priorities);
    }

    // Next, compile a list of device extensions & features to enable
    Tools::Array<const char*> vk_device_extensions           = this->instance.get_device_extensions();
    Tools::Array<Vulkanic::DeviceFeature> device_features = this->instance.get_device_features();
    // Convert the features to Vulkan features
    VkPhysicalDeviceFeatures vk_device_features;
    populate_device_features(vk_device_features, device_features);

    // Now we can populate the create info for the device itself
    VkDeviceCreateInfo device_info;
    populate_device_info(device_info, queue_infos, vk_device_extensions, vk_device_features);

    // Create the Device
    VkResult vk_result;
    if ((vk_result = vkCreateDevice(physical_device, &device_info, nullptr, &this->vk_device)) != VK_SUCCESS) {
        logger.fatalc(Device::channel, "Cannot create Vulkan device: ", Vulkanic::vk_error_map.at(vk_result));
    }

    // With the device created, pull the queues from it
    for (uint32_t i = 0; i < queue_family_map.size(); i++) {
        // Make sure there's enough space in the queue list first
        this->queues[i].resize(queue_family_map[i].second);
        // Loop through the queue count to retrieve each of them
        for (uint32_t j = 0; j < queue_family_map[i].second; j++) {
            vkGetDeviceQueue(this->vk_device, queue_family_map[i].first, j, &this->queues[i][j]);
        }
    }

    // Done!
}

/* Move constructor for the Device class. */
Device::Device(Device&& other) :
    instance(other.instance),

    physical_device(std::move(other.physical_device)),

    vk_device(other.vk_device),
    queues(std::move(other.queues))
{
    other.vk_device = nullptr;
}

/* Destructor for the Device class. */
Device::~Device() {
    if (this->vk_device != nullptr) {
        vkDestroyDevice(this->vk_device, nullptr);
    }
}



/* Swap operator for the Device class. */
void Makma3D::swap(Device& d1, Device& d2) {
    #ifndef NDEBUG
    if (&d1.instance != &d2.instance) { logger.fatalc(Device::channel, "Cannot swap devices with different instances."); }
    #endif

    using std::swap;

    swap(d1.physical_device, d2.physical_device);

    swap(d1.vk_device, d2.vk_device);
    swap(d1.queues, d2.queues);
}
