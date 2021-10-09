/* WINDOW.cpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:24:09
 * Last edited:
 *   08/10/2021, 14:24:09
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the GLFW Window derivative, that uses the GLFW library to
 *   create a window and a surface, after which the normal Window class can
 *   build a swapchain around it.
**/

#include <limits>

#include "tools/Logger.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "glfw/Window.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GLFW;


/***** WINDOW CLASS *****/
/* Constructor for the Window class. */
Window::Window(const GLFW::Instance& glfw_instance, const Vulkanic::Instance& vulkanic_instance, const GLFW::Monitor* monitor, const std::string& title, const VkExtent2D& extent, Windowing::WindowMode mode) :
    Windowing::Window(monitor, title, extent, mode),
    instance(glfw_instance)
{
    // Make sure the monitor is valid
    if (this->_monitor != nullptr && !this->_allowed_monitor(this->_monitor)) {
        logger.fatalc(Window::channel, "Invalid monitor for ", this->_api_name(), " backend.");
    }

    // Set whether this Window is resizeable or not
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create the window differently based on the window mode
    switch(this->_mode) {
    case Windowing::WindowMode::windowed:
        if (this->_monitor != nullptr) {
            logger.warningc(Window::channel, "Monitor given unnecessarily for Windowed window mode; ignoring.");
            delete this->_monitor;
            this->_monitor = nullptr;
        }
        this->glfw_window = glfwCreateWindow(static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height), this->_title.c_str(), NULL, NULL);
        break;

    case Windowing::WindowMode::fullscreen:
        if (this->_monitor == nullptr) { logger.fatalc(Window::channel, "Missing monitor for Fullscreen window mode."); }
        this->glfw_window = glfwCreateWindow(static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height), this->_title.c_str(), monitor->glfw(), NULL);
        break;

    case Windowing::WindowMode::windowed_fullscreen: {
        if (this->_monitor == nullptr) { logger.fatalc(Window::channel, "Missing monitor for Windowed Fullscreen window mode."); }

        // Overwrite the internal size with the one given by the video mode
        const GLFWvidmode* video_mode = ((const GLFW::Monitor*) this->_monitor)->idle_video_mode();
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
        logger.fatalc(Window::channel, "Unsupported WindowMode '", Windowing::window_mode_names[(int) this->_mode], "'.");

    }

    // With the Window created, create the Vulkan surface
    VkResult vk_result;
    VkSurfaceKHR vk_surface;
    if((vk_result = glfwCreateWindowSurface(vulkanic_instance, this->glfw_window, nullptr, &vk_surface)) != VK_SUCCESS) {
        logger.fatalc(Window::channel, "Could not create window surface: ", Vulkanic::vk_error_map[vk_result]);
    }

    // Get its framebuffer size
    int fw, fh;
    glfwGetFramebufferSize(this->glfw_window, &fw, &fh);

    // With that, create the Surface object and set it internally
    this->_surface = new Vulkanic::Surface(vulkanic_instance, vk_surface, { static_cast<uint32_t>(fw), static_cast<uint32_t>(fh) });

    // Do a success print
    if (logger.get_verbosity() >= Verbosity::debug) {
        switch(this->_mode) {
        case Windowing::WindowMode::windowed:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Windowed mode.");
            break;

        case Windowing::WindowMode::fullscreen:
            logger.logc(Verbosity::important, Window::channel, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Fullscreen mode on monitor ", this->_monitor->index(), '.');
            break;

        case Windowing::WindowMode::windowed_fullscreen:
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
    Windowing::Window(std::move(other)),
    instance(other.instance),

    glfw_window(other.glfw_window)
{
    other.glfw_window = nullptr;
}

/* Destructor for the Window class. */
Window::~Window() {
    if (this->glfw_window != nullptr) {
        glfwDestroyWindow(this->glfw_window);
    }
}



/* Returns the backend API's name. */
const char* Window::_api_name() const {
    return "GLFW";
}

/* Checks if the given monitor makes sense for this API backend. */
bool Window::_allowed_monitor(const Windowing::Monitor* monitor) const {
    return dynamic_cast<const GLFW::Monitor*>(monitor) != nullptr;
}

/* Returns the nearest monitor to the current Window position. Only called if the current mode is windowed. */
const Windowing::Monitor* Window::_find_nearest_monitor() const {
    // Get the window's position and size in the virtual screen
    int window_x, window_y, window_w, window_h;
    glfwGetWindowPos(this->glfw_window, &window_x, &window_y);
    glfwGetWindowSize(this->glfw_window, &window_w, &window_h);

    // Loop through the available monitors
    int best_area = 0;
    uint32_t best_monitor = std::numeric_limits<uint32_t>::max();
    const Tools::Array<GLFW::Monitor> monitors = this->instance.get_monitors();
    for (uint32_t i = 0; i < monitors.size(); i++) {
        // Get the monitor's position and size in the virtual screen
        int monitor_x, monitor_y, monitor_w, monitor_h;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);
        monitor_w = monitors[i].current_video_mode()->width;
        monitor_h = monitors[i].current_video_mode()->height;

        // Compare the two by computing the overlapping area
        int area = max(0, min(window_x + window_w, monitor_x + monitor_w) - max(window_x, monitor_x)) *
                   max(0, min(window_y + window_h, monitor_y + monitor_h) - max(window_y, monitor_y));
        if (area > best_area) {
            best_area = area;
            best_monitor = i;
        }
    }

    // If the best monitor is not found, then throw a warning but do return
    if (best_monitor == std::numeric_limits<uint32_t>::max()) { logger.warningc(Window::channel, "No best Monitor found for windowed window."); }

    // Done
    return (const Windowing::Monitor*) &monitors[best_monitor];
}



/* Replace the backend monitor with the internal one. The new size (if relevant) and the new window mode are already set internally at this point. */
void Window::_replace_monitor() {
    const GLFW::Monitor* glfw_monitor = (const GLFW::Monitor*) this->_monitor;

    // If we're in windowed fullscreen, set a specific size and refresh rate
    int refresh_rate = GLFW_DONT_CARE;
    if (this->_mode == Windowing::WindowMode::windowed_fullscreen) {
        this->_extent.width = static_cast<uint32_t>(glfw_monitor->idle_video_mode()->width);
        this->_extent.height = static_cast<uint32_t>(glfw_monitor->idle_video_mode()->height);
        refresh_rate = glfw_monitor->idle_video_mode()->refreshRate;
    }

    // Push the monitor change to the GLFW backend
    glfwSetWindowMonitor(
        // The Window and its new Monitor
        this->glfw_window, glfw_monitor->glfw(),
        // The position of the Window (ignored when setting a new monitor)
        GLFW_DONT_CARE, GLFW_DONT_CARE,
        // The new size of the Window (we just use the old one)
        static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
        // The refresh rate
        refresh_rate
    );

    // Done
    logger.logc(Verbosity::important, Window::channel, "Moved window to monitor ", glfw_monitor->index(), " (", glfw_monitor->name(), ", ", glfw_monitor->resolution(), ").");
}

/* Replace the title of the backend monitor with the internal one. */
void Window::_replace_title() {
    // Simply push
    glfwSetWindowTitle(this->glfw_window, this->_title.c_str());
    logger.logc(Verbosity::important, Window::channel, "Changed window title to '", this->_title, "'.");
}

/* Resize the internal Window to the internal extent. */
void Window::_resize_window() {
    // Simply call the GLFW function for this
    glfwSetWindowSize(this->glfw_window, static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height));
    logger.logc(Verbosity::important, Window::channel, "Resized window to ", this->_extent, '.');
}



/* Sets the Window to the windowed mode. Guaranteed to not be called in the case the Window is already windowed. The new window size is already set internally. The internal Monitor is not yet destroyed at this point, but will be after this call returns. */
void Window::_make_windowed(const VkOffset2D& new_pos) {
    // Simply set the monitor with the desired properties
    glfwSetWindowMonitor(
        // The Window and its new Monitor (no monitor, so go to windowed)
        this->glfw_window, NULL,
        // The position of the Window
        static_cast<int>(new_pos.x), static_cast<int>(new_pos.y),
        // The new size of the Window (we just use the old one)
        static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
        // The refresh rate (which we can ignore here)
        GLFW_DONT_CARE
    );
    logger.logc(Verbosity::important, Window::channel, "Set window mode to Windowed.");
}

/* Sets the Window to the fullscreen mode. Guaranteed to not be called in case the Window is already in fullscreen. The new window size is already set internally, just like the new Monitor. */
void Window::_make_fullscreen() {
    const GLFW::Monitor* glfw_monitor = (const GLFW::Monitor*) this->_monitor;

    // Simply set the monitor with the desired properties
    glfwSetWindowMonitor(
        // The Window and its new Monitor
        this->glfw_window, glfw_monitor->glfw(),
        // The position of the Window (which we ignore)
        GLFW_DONT_CARE, GLFW_DONT_CARE,
        // The new size of the Window (we just use the old one)
        static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
        // The refresh rate (which we can ignore here too)
        GLFW_DONT_CARE
    );
    logger.logc(Verbosity::important, Window::channel, "Set window mode to Fullscreen (", this->_extent, ", on monitor ", glfw_monitor->index(), " ('", glfw_monitor->name(), "')).");
}

/* Sets the Window to the windowed fullscreen mode. Guaranteed to not be called in case the Window is already in fullscreen. The new monitor is already set internally, and the rest of the properties we copy from that. */
void Window::_make_windowed_fullscreen() {
    const GLFW::Monitor* glfw_monitor = (const GLFW::Monitor*) this->_monitor;

    // Replace the size internally
    this->_extent.width = static_cast<uint32_t>(glfw_monitor->idle_video_mode()->width);
    this->_extent.height = static_cast<uint32_t>(glfw_monitor->idle_video_mode()->height);

    // Simply set the monitor with the desired properties
    glfwSetWindowMonitor(
        // The Window and its new Monitor
        this->glfw_window, glfw_monitor->glfw(),
        // The position of the Window (which we ignore)
        GLFW_DONT_CARE, GLFW_DONT_CARE,
        // The new size of the Window (take the ones copies from the monitor)
        static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height),
        // The refresh rate, which we take from the monitor
        glfw_monitor->idle_video_mode()->refreshRate
    );
    logger.logc(Verbosity::important, Window::channel, "Set window mode to Windowed Fullscreen (", this->_extent, ", on monitor ", glfw_monitor->index(), " ('", glfw_monitor->name(), "')).");
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



/* Does a single pass of the window events for this window. Returns whether the window should stay open (true) or not (false). */
bool Window::loop() const {
    // First, poll the GLFW events
    glfwPollEvents();

    // Next, return if the Window should close
    return !glfwWindowShouldClose(this->glfw_window);
}



/* Swap operator for the Window class. */
void GLFW::swap(Window& w1, Window& w2) {
    #ifndef NDEBUG
    if (&w1.instance != &w2.instance) { logger.fatalc(Window::channel, "Cannot swap windows with different instances."); }
    #endif

    using std::swap;

    swap((Windowing::Window&) w1, (Windowing::Window&) w2);
    swap(w1.glfw_window, w2.glfw_window);
}
