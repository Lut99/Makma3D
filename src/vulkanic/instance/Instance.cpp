/* INSTANCE.cpp
 *   by Lut99
 *
 * Created:
 *   30/04/2021, 14:03:33
 * Last edited:
 *   25/05/2021, 18:14:13
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Instance class, which wraps the Vulkan instance and
 *   manages the debugger.
**/

#include <cstring>

#include "tools/Logger.hpp"

#include "vulkanic/auxillary/ErrorCodes.hpp"
#include "vulkanic/instance/Instance.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Vulkanic;


/***** CONSTANTS *****/
/* Channel name for Vulkan messages. */
static constexpr const char* vulkan_channel = "Vulkan";





/***** DEBUG CALLBACK *****/
static VKAPI_ATTR VkBool32 VKAPI_CALL vk_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                  void* user_data)
{
    // Do not use the message type for now
    (void) message_type;

    // Log the message with the correct severity and message type
    Tools::Logger* logger = (Tools::Logger*) user_data;
    switch(message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            logger->logc(Verbosity::details, vulkan_channel, pCallbackData->pMessage, " (ID: '", pCallbackData->pMessageIdName, "')");
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            logger->warningc(vulkan_channel, pCallbackData->pMessage, " (ID: '", pCallbackData->pMessageIdName, "')");
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            logger->errorc(vulkan_channel, pCallbackData->pMessage, " (ID: '", pCallbackData->pMessageIdName, "')");
            break;

        default:
            // Throw meta error
            logger->fatalc(Instance::channel, "Unknown Vulkan message severity.");
            break;

    }
    return VK_FALSE;
}





/***** POPULATE FUNCTIONS *****/
/* Populates a VkApplicationInfo struct with the application info we hardcoded here. */
static void populate_application_info(VkApplicationInfo& app_info, const char* application_name, uint32_t application_version, uint32_t makma_version) {
    // Set the struct to 0 and set its type
    app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    // Next, set the name & version info of the application
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = application_version;

    // We're not using any well-known engine, so tell Vulkan we're not
    app_info.pEngineName = "Makma3D";
    app_info.engineVersion = makma_version;

    // Finally, define the minimum API level to use
    app_info.apiVersion = VK_API_VERSION_1_0;
}

/* Populates a VkInstanceCreateInfo struct with the given list of extensions and layers. If you don't want to enable either of those, pass empty lists. */
static void populate_instance_info(VkInstanceCreateInfo& instance_info, const VkApplicationInfo& app_info, const Tools::Array<const char*>& extensions, const Tools::Array<const char*>& layers) {
    // Set the struct to 0 and set its type
    instance_info = {};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Link the application info to this instance
    instance_info.pApplicationInfo = &app_info;

    // Set the extensions
    instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_info.ppEnabledExtensionNames = extensions.size() > 0 ? extensions.rdata() : nullptr;

    // Set the layers
    instance_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instance_info.ppEnabledLayerNames = layers.size() > 0 ? layers.rdata() : nullptr;
}

/* Populates a VkDebugUtilsMessengerCreateInfoEXT struct with the hardcoded settings. */
static void populate_debug_info(VkDebugUtilsMessengerCreateInfoEXT& debug_info) {
    // Set the struct to 0 and set its type
    debug_info = {};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    // Note which debug sevirities & message types we want to log
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    // Define the callback with an optional data struct (the Logger itself)
    debug_info.pfnUserCallback = vk_callback;
    debug_info.pUserData = (void*) &logger;
}





/***** DYNAMIC LOADER FUNCTIONS *****/
/* Loads a given method ('s address) from the given instance. */
static PFN_vkVoidFunction load_instance_method(VkInstance vk_instance, const char* method_name) {
    // Fetch the function pointer
    PFN_vkVoidFunction to_return = vkGetInstanceProcAddr(vk_instance, method_name);
    if (to_return == nullptr) {
        logger.fatalc(Instance::channel, "Could not load function '", method_name, "'.");
    }

    // Otherwise, log success and return
    logger.logc(Tools::Verbosity::debug, Instance::channel, "Loaded function '", method_name, "'.");
    return to_return;
}





/***** INSTANCE CLASS *****/
/* Constructor for the Instance class. */
Instance::Instance() :
    vk_instance(nullptr),
    vk_debugger(nullptr),
    vk_destroy_debug_utils_messenger_method(nullptr)
{}

/* Move constructor for the Instance class. */
Instance::Instance(Instance&& other) :
    vk_instance(other.vk_instance),

    vk_debugger(other.vk_debugger),
    vk_destroy_debug_utils_messenger_method(other.vk_destroy_debug_utils_messenger_method)
{
    // Set everything to nullptrs in the other function to avoid deallocation
    other.vk_instance = nullptr;
    other.vk_debugger = nullptr;
}

/* Destructor for the Instance class. */
Instance::~Instance() {
    // Destroy the debugger
    if (this->vk_debugger != nullptr) {
        this->vk_destroy_debug_utils_messenger_method(this->vk_instance, this->vk_debugger, nullptr);
    }

    // Finally, destroy the instance
    if (this->vk_instance != nullptr) {
        vkDestroyInstance(this->vk_instance, nullptr);
    }
}



/* Initializes the instance. */
void Instance::init(const char* application_name, uint32_t application_version, uint32_t makma_version, const Tools::Array<const char*>& extensions, const Tools::Array<const char*>& layers) {
    logger.logc(Verbosity::details, Instance::channel, "Initializing Vulkan...");

    // Defining the app & engine description
    VkApplicationInfo app_info;
    populate_application_info(app_info, application_name, application_version, makma_version);

    // Setup the list of extensions
    VkInstanceCreateInfo instance_info;
    populate_instance_info(instance_info, app_info, extensions, layers);

    // Now, setup the instance
    VkResult vk_result;
    if ((vk_result = vkCreateInstance(&instance_info, nullptr, &this->vk_instance)) != VK_SUCCESS) {
        logger.fatalc("Could not create the Vulkan instance: ", Instance::channel, vk_error_map[vk_result]);
    }

    // If we were successfull, print which extensions & layers
    if (logger.get_verbosity() >= Verbosity::debug) {
        for (uint32_t i = 0; i < extensions.size(); i++) {
            logger.logc(Verbosity::debug, Instance::channel, "Enabled Vulkan extension '", extensions[i], "'.");
        }
        for (uint32_t i = 0; i < layers.size(); i++) {
            logger.logc(Verbosity::debug, Instance::channel, "Enabled Vulkan layer '", layers[i], "'.");
        }
    }
}

/* Initializes the debugging part of the instance. */
void Instance::init_debug() {
    logger.logc(Verbosity::details, Instance::channel, "Enabling Vulkan debugger...");

    // First, we load the two extension functions needed using the dynamic loader
    PFN_vkCreateDebugUtilsMessengerEXT vk_create_debug_utils_messenger_method = (PFN_vkCreateDebugUtilsMessengerEXT) load_instance_method(this->vk_instance, "vkCreateDebugUtilsMessengerEXT");
    this->vk_destroy_debug_utils_messenger_method = (PFN_vkDestroyDebugUtilsMessengerEXT) load_instance_method(this->vk_instance, "vkDestroyDebugUtilsMessengerEXT");

    // Next, define the messenger
    VkDebugUtilsMessengerCreateInfoEXT debug_info;
    populate_debug_info(debug_info);

    // And with that, create it
    VkResult vk_result;
    if ((vk_result = vk_create_debug_utils_messenger_method(this->vk_instance, &debug_info, nullptr, &this->vk_debugger)) != VK_SUCCESS) {
        logger.fatalc(Instance::channel, "Could not create the logger: ", vk_error_map[vk_result]);
    }
}



/* Returns the list of (supported) PhysicalDevices that are currently registered to the Vulkan backend. */
Tools::Array<PhysicalDevice> Instance::get_physical_devices(VkSurfaceKHR vk_surface, const Tools::Array<const char*>& vk_device_extensions, const Tools::Array<Vulkanic::DeviceFeature>& vk_device_features) const {
    // Get the devices from Vulkan
    uint32_t n_physical_devices;
    vkEnumeratePhysicalDevices(this->vk_instance, &n_physical_devices, nullptr);
    if (n_physical_devices == 0) { logger.warningc(Instance::channel, "No Vulkan-capable devices found."); return {}; }
    Tools::Array<VkPhysicalDevice> physical_devices(n_physical_devices);
    vkEnumeratePhysicalDevices(this->vk_instance, &n_physical_devices, physical_devices.wdata(n_physical_devices));

    // For each of them, check if they are compatible, and add them to the list as a PhysicalDevice if they are
    Tools::Array<PhysicalDevice> result(n_physical_devices);
    for (uint32_t i = 0; i < n_physical_devices; i++) {
        if (PhysicalDevice::is_suitable(physical_devices[i], vk_surface, vk_device_extensions, vk_device_features)) {
            result.push_back(PhysicalDevice(physical_devices[i], i));
        }
    }

    // Done, return
    return result;
}



/* Swap operator for the Instance class. */
void Vulkanic::swap(Instance& i1, Instance& i2) {
    using std::swap;

    swap(i1.vk_instance, i2.vk_instance);

    swap(i1.vk_debugger, i2.vk_debugger);
    swap(i1.vk_destroy_debug_utils_messenger_method, i2.vk_destroy_debug_utils_messenger_method);
}
