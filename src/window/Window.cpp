/* WINDOW.cpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 15:57:57
 * Last edited:
 *   06/10/2021, 15:57:57
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Window class, which represents a single Window. Is based
 *   on some backend API (probably GLFW), and provides interaction in terms
 *   of both rendering and user input.
**/

#include "tools/Logger.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "window/Window.hpp"

using namespace std;
using namespace Makma3D;


/***** WINDOW CLASS *****/
/* Constructor for the Window class. */
Window::Window(const Makma3D::Instance& instance, const Monitor* monitor, const std::string& title, const VkExtent2D& extent, WindowMode mode) :
    instance(instance),

    _monitor(monitor),

    _title(title),
    _extent(extent),
    _mode(mode)
{
    // Set whether this Window is resizeable or not
    glfwWindowHint(GLFW_RESIZABLE, mode == WindowMode::windowed_resizeable);

    // Create the window differently based on the window mode
    switch(this->_mode) {
    case WindowMode::windowed:
    case WindowMode::windowed_resizeable:
        if (this->_monitor != nullptr) {
            logger.warningc(Window::channel, "Monitor given unnecessarily for Windowed window mode; ignoring.");
            delete this->_monitor;
            this->_monitor = nullptr;
        }
        this->glfw_window = glfwCreateWindow(static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height), this->_title.c_str(), NULL, NULL);
        break;

    case WindowMode::fullscreen:
        if (this->_monitor == nullptr) { logger.fatalc(Window::channel, "Missing monitor for Fullscreen window mode."); }
        this->glfw_window = glfwCreateWindow(static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height), this->_title.c_str(), monitor->glfw(), NULL);
        break;

    case WindowMode::windowed_fullscreen: {
        if (this->_monitor == nullptr) { logger.fatalc(Window::channel, "Missing monitor for Windowed Fullscreen window mode."); }

        // Overwrite the internal size with the one given by the video mode
        const GLFWvidmode* video_mode = this->_monitor->idle_video_mode();
        this->_extent.width  = static_cast<uint32_t>(video_mode->width);
        this->_extent.height = static_cast<uint32_t>(video_mode->height);

        // Set the video mode for this window first
        glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

        // Now create the window
        this->glfw_window = glfwCreateWindow(video_mode->width, video_mode->height, this->_title.c_str(), monitor->glfw(), NULL);
        break;
    }

    default:
        logger.fatalc(Window::channel, "Unsupported WindowMode '", window_mode_names[(int) this->_mode], "'.");

    }

    // With the Window created, create the Vulkan surface
    VkResult vk_result;
    VkSurfaceKHR vk_surface;
    if((vk_result = glfwCreateWindowSurface(this->instance, this->glfw_window, nullptr, &vk_surface)) != VK_SUCCESS) {
        logger.fatalc(Window::channel, "Could not create window surface: ", Vulkanic::vk_error_map[vk_result]);
    }

    // Get its framebuffer size
    int fw, fh;
    glfwGetFramebufferSize(this->glfw_window, &fw, &fh);

    // With that, create the Surface object and set it internally
    this->_surface = new Vulkanic::Surface(this->instance, vk_surface, { static_cast<uint32_t>(fw), static_cast<uint32_t>(fh) });

    // Do a success print
    if (logger.get_verbosity() >= Verbosity::debug) {
        switch(this->_mode) {
        case WindowMode::windowed:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Windowed mode.");
            break;
            
        case WindowMode::windowed_resizeable:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Windowed Resizeable mode.");
            break;

        case WindowMode::fullscreen:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Fullscreen mode on monitor ", this->_monitor->index(), '.');
            break;

        case WindowMode::windowed_fullscreen:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in WindowedFullscreen mode on monitor ", this->_monitor->index(), '.');
            break;

        default:
            // Ignore
            break;

        }
    }

    // Done!
}

/* Move constructor for the Window class. */
Window::Window(Window&& other) :
    instance(other.instance),

    glfw_window(other.glfw_window),

    _monitor(other._monitor),

    _title(other._title),
    _extent(other._extent),
    _mode(other._mode),

    _surface(other._surface)
{
    other.glfw_window = nullptr;
    other._surface = nullptr;
}

/* Destructor for the Window class. */
Window::~Window() {
    // Delete the swapchain

    // Delete the surface
    if (this->_surface != nullptr) {
        delete this->_surface;
    }

    // Destroy the window
    if (this->glfw_window != nullptr) {
        glfwDestroyWindow(this->glfw_window);
    }

    // Do some nice debug print
    logger.logc(Verbosity::important, Window::channel, "Destroyed Window.");
}



/* Returns the nearest monitor to the current Window position. Only called if the current mode is windowed. */
const Monitor* Window::_find_nearest_monitor() const {
    // Get the window's position and size in the virtual screen
    int window_x, window_y, window_w, window_h;
    glfwGetWindowPos(this->glfw_window, &window_x, &window_y);
    glfwGetWindowSize(this->glfw_window, &window_w, &window_h);

    // Loop through the available monitors
    int best_area = 0;
    uint32_t best_monitor = std::numeric_limits<uint32_t>::max();
    const Tools::Array<const Monitor*>& monitors = this->instance.get_monitors();
    for (uint32_t i = 0; i < monitors.size(); i++) {
        // Get the monitor's position and size in the virtual screen
        const VkOffset2D& offset = monitors[i]->scaled_offset();
        const VkExtent2D& extent = monitors[i]->scaled_resolution();

        // Compare the two by computing the overlapping area
        int area = max(0, min(window_x + window_h, static_cast<int>(offset.x) + static_cast<int>(extent.width)) - max(window_x, static_cast<int>(offset.x))) *
                   max(0, min(window_y + window_h, static_cast<int>(offset.y) + static_cast<int>(extent.height)) - max(window_y, static_cast<int>(offset.y)));
        if (area > best_area) {
            best_area = area;
            best_monitor = i;
        }
    }

    // If the best monitor is not found, then throw a warning but do return
    if (best_monitor == std::numeric_limits<uint32_t>::max()) { logger.warningc(Window::channel, "No best Monitor found for windowed window."); }

    // Done
    return monitors[best_monitor];
}

/* Reconstruct the internal Surface from the internal Window. */
void Window::_reconstruct_surface() {
    // Re-create the VkSurfaceKHR first
    VkResult vk_result;
    VkSurfaceKHR vk_surface;
    if((vk_result = glfwCreateWindowSurface(this->_surface->instance, this->glfw_window, nullptr, &vk_surface)) != VK_SUCCESS) {
        logger.fatalc(Window::channel, "Could not re-create window surface: ", Vulkanic::vk_error_map[vk_result]);
    }

    // Get its framebuffer size
    int fw, fh;
    glfwGetFramebufferSize(this->glfw_window, &fw, &fh);

    // Re-create the internal Surface too
    this->_surface->recreate(vk_surface, { static_cast<uint32_t>(fw), static_cast<uint32_t>(fh) });
}



// /* Uses the given GPU to create the internal swapchain. Must be called before the window can be rendered to, obviously. */
// void Window::bind(const Vulkanic::GPU& gpu) {

// }



/* Does a single pass of the window events for this window. Returns whether the window should stay open (true) or not (false). */
bool Window::loop() const {
    // First, poll the GLFW events
    glfwPollEvents();

    // Next, return if the Window should close
    return !glfwWindowShouldClose(this->glfw_window);
}



/* Sets the monitor of the Window, giving it a new size while at it. Only relevant when the Window is not in windowed mode (does nothing if it is). */
void Window::set_monitor(const Monitor* new_monitor, const VkExtent2D& new_extent) {
    // If the current mode is Windowed, do nothing
    if (this->_mode == WindowMode::windowed) { return; }

    // Set the monitor internally
    this->_monitor = new_monitor;

    // Determine which size to get for the update
    int refresh_rate = GLFW_DONT_CARE;
    if (this->_mode == WindowMode::windowed_fullscreen) {
        this->_extent = { static_cast<uint32_t>(this->_monitor->idle_video_mode()->width), static_cast<uint32_t>(this->_monitor->idle_video_mode()->height) };
        refresh_rate = this->_monitor->idle_video_mode()->refreshRate;
    } else {
        this->_extent = new_extent;
    }

    // Push the monitor change to the GLFW backend
    glfwSetWindowMonitor(
        // The Window and its new Monitor
        this->glfw_window, this->_monitor->glfw(),
        // The position of the Window (ignored when setting a new monitor)
        GLFW_DONT_CARE, GLFW_DONT_CARE,
        // The new size of the Window (we just use the old one)
        static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
        // The refresh rate
        refresh_rate
    );

    // We now have to rebuild the surface & swapchain
    this->_reconstruct_surface();
    /* TBD */

    // Done
    logger.logc(Verbosity::important, Window::channel, "Moved window to monitor ", this->_monitor->index(), " (", this->_monitor->name(), ", ", this->_monitor->resolution(), ").");
}

/* Sets the title of the Window. */
void Window::set_title(const std::string& new_title) {
    // Set the title internally
    this->_title = new_title;
    // Also push to GLFW
    glfwSetWindowTitle(this->glfw_window, this->_title.c_str());
    // Done
    logger.logc(Verbosity::important, Window::channel, "Changed window title to '", this->_title, "'.");
}

/* Resizes the Window to a new extent. Does nothing if the Window is in windowed fullscreen mode. */
void Window::set_extent(const VkExtent2D& new_extent) {
    // Set the extent internally
    this->_extent = new_extent;
    // Also update GLFW's knowledge of this
    glfwSetWindowSize(this->glfw_window, static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height));
    // We now have to rebuild the surface & swapchain
    this->_reconstruct_surface();
    /* TBD */
    // Done
    logger.logc(Verbosity::important, Window::channel, "Resized window to ", this->_extent, '.');
}

/* Changes the Window mode to the given one. */
void Window::set_mode(WindowMode new_mode, const VkExtent2D new_extent, const Monitor* new_monitor) {
    // Check if the mode is different
    if (this->_mode == new_mode) { return; }

    // Try to resolve the monitor if needed
    if (new_mode != WindowMode::windowed) {
        // See if we need to resolve the monitor
        if (new_monitor == nullptr) {
            // We do; either grab the existing one or find the nearest
            if (this->_mode == WindowMode::windowed) {
                new_monitor = this->_find_nearest_monitor();
            } else {
                new_monitor = this->_monitor;
            }
        }
    }

    // Store the extent if needed
    if (new_mode != WindowMode::windowed_fullscreen) {
        this->_extent = new_extent;
    }

    // With the monitor resolved, transition to the new mode
    if (new_mode == WindowMode::windowed) {
        // Compute the middle of the monitor
        VkRect2D workarea = this->_monitor->scaled_workarea();
        int middle_x = static_cast<int>(workarea.offset.x) + static_cast<int>(workarea.extent.width / 2) - static_cast<int>(this->_extent.width / 2);
        int middle_y = static_cast<int>(workarea.offset.y) + static_cast<int>(workarea.extent.height / 2) - static_cast<int>(this->_extent.height / 2);

        // Simply set the monitor with the desired properties
        glfwSetWindowMonitor(
            // The Window and its new Monitor (no monitor, so go to windowed)
            this->glfw_window, NULL,
            // The position of the Window
            middle_x, middle_y,
            // The new size of the Window (we just use the old one)
            static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
            // The refresh rate (which we can ignore here)
            GLFW_DONT_CARE
        );
        // Remove the internal monitor
        this->_monitor = nullptr;

    } else if (new_mode == WindowMode::fullscreen) {
        // Set the new monitor
        this->_monitor = new_monitor;

        // Simply set the monitor with the desired properties
        glfwSetWindowMonitor(
            // The Window and its new Monitor
            this->glfw_window, this->_monitor->glfw(),
            // The position of the Window (which we ignore)
            GLFW_DONT_CARE, GLFW_DONT_CARE,
            // The new size of the Window (we just use the old one)
            static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
            // The refresh rate (which we can ignore here too)
            GLFW_DONT_CARE
        );

    } else if (new_mode == WindowMode::windowed_fullscreen) {
        // Set the new monitor
        this->_monitor = new_monitor;

        // Replace the size internally
        this->_extent.width = static_cast<uint32_t>(this->_monitor->idle_video_mode()->width);
        this->_extent.height = static_cast<uint32_t>(this->_monitor->idle_video_mode()->height);

        // Simply set the monitor with the desired properties
        glfwSetWindowMonitor(
            // The Window and its new Monitor
            this->glfw_window, this->_monitor->glfw(),
            // The position of the Window (which we ignore)
            GLFW_DONT_CARE, GLFW_DONT_CARE,
            // The new size of the Window (take the ones copies from the monitor)
            static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
            // The refresh rate, which we take from the monitor
            this->_monitor->idle_video_mode()->refreshRate
        );
        
    } else {
        logger.fatalc(Window::channel, "Unsupported WindowMode '", window_mode_names[(int) new_mode], "'.");
    }

    // Finally, don't forget to update the mode
    this->_mode = new_mode;

    // With the mode changed, reconstruct the surface and the swapchain
    this->_reconstruct_surface();
    /* TBD */

    // Show the ending log
    switch(new_mode) {
    case WindowMode::windowed:
        logger.logc(Verbosity::important, Window::channel, "Set window mode to Windowed.");
        break;

    case WindowMode::windowed_resizeable:
        logger.logc(Verbosity::important, Window::channel, "Set window mode to Windowed Resizeable.");
        break;

    case WindowMode::fullscreen:
        logger.logc(Verbosity::important, Window::channel, "Set window mode to Fullscreen (", this->_extent, ", on monitor ", this->_monitor->index(), " ('", this->_monitor->name(), "')).");
        break;

    case WindowMode::windowed_fullscreen:
        logger.logc(Verbosity::important, Window::channel, "Set window mode to Windowed Fullscreen (", this->_extent, ", on monitor ", this->_monitor->index(), " ('", this->_monitor->name(), "')).");
        break;

    default:
        break;

    }

    // Done!
}



/* Returns the physical device that the library thinks is most suited for this window. */
PhysicalDevice Window::get_preferred_physical_device(PhysicalDeviceType preferred_type) const {
    // Get the list of physical devices
    Tools::Array<PhysicalDevice> physical_devices = this->get_physical_devices();
    if (physical_devices.empty()) {
        logger.fatalc(Window::channel, "No supported devices found for Window '", this->_title, "'.");
    }

    // Go through it to find one with the preferred type
    for (uint32_t i = 0; i < physical_devices.size(); i++) {
        if (physical_devices[i].type() == preferred_type) {
            return physical_devices[i];
        }
    }

    // Otherwise, no such device found; return the first
    return physical_devices.first();
}

/* Returns the list of (supported) PhysicalDevices that can render to this Window. */
Tools::Array<PhysicalDevice> Window::get_physical_devices() const {
    // First, compile a list of device extensions & features to enable based on the enabled Makma3D extensions
    Tools::Array<const char*> vk_device_extensions = {};
    Tools::Array<Vulkanic::DeviceFeature> vk_device_features = { Vulkanic::DeviceFeature::anisotropy };
    for (uint32_t i = 0; i < this->instance.extensions.size(); i++) {
        /* None, as of yet */
    }

    // Call the Vulkan instance's version of this function
    return this->instance.vk_instance.get_physical_devices(this->_surface->vk(), vk_device_extensions, vk_device_features);
}



/* Swap operator for the Window class. */
void Makma3D::swap(Window& w1, Window& w2) {
    using std::swap;

    swap(w1.glfw_window, w2.glfw_window);

    swap(w1._monitor, w2._monitor);

    swap(w1._title, w2._title);
    swap(w1._extent, w2._extent);
    swap(w1._mode, w2._mode);
    
    swap(w1._surface, w2._surface);
}
