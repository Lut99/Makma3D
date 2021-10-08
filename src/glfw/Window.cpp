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

#include "tools/Logger.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "glfw/Window.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::GLFW;


/***** WINDOW CLASS *****/
/* Constructor for the Window class, which takes a Vulkananic instance to initialize the Surface with, the Window's title, its size as a VkExtend2D and its fullscreen mode. If the window mode if 'windowed fullscreen', ignores the given size and uses the monitor's size instead. If the window mode isn't 'windowed', then a Monitor must be provided to spawn the Window on. Copies the monitor if given. */
Window::Window(const Vulkanic::Instance& instance, const std::string& title, const VkExtent2D& extent, Windowing::WindowMode mode, const GLFW::Monitor* monitor) :
    Windowing::Window(title, extent, mode, (const Windowing::Monitor*) new GLFW::Monitor(*monitor))
{
    // Set whether this Window is resizeable or not
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Store the current video mode first to be able to later switch to windowed mode
    if (this->_monitor != nullptr) {
        this->glfw_video_mode = glfwGetVideoMode(((const GLFW::Monitor*) this->_monitor)->glfw());
    } else {
        this->glfw_video_mode = nullptr;
    }

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
        this->_extent.width  = static_cast<uint32_t>(this->glfw_video_mode->width);
        this->_extent.height = static_cast<uint32_t>(this->glfw_video_mode->height);

        // Set the video mode for this window first
        glfwWindowHint(GLFW_RED_BITS, this->glfw_video_mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, this->glfw_video_mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, this->glfw_video_mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, this->glfw_video_mode->refreshRate);

        // Now create the window
        this->glfw_window = glfwCreateWindow(this->glfw_video_mode->width, this->glfw_video_mode->height, this->_title.c_str(), monitor->glfw(), NULL);
        break;
    }

    default:
        logger.fatalc(Window::channel, "Unsupported WindowMode '", Windowing::window_mode_names[(int) this->_mode], "'.");

    }

    // With the Window created, create the Vulkan surface
    VkResult vk_result;
    VkSurfaceKHR vk_surface;
    if((vk_result = glfwCreateWindowSurface(instance, this->glfw_window, nullptr, &vk_surface)) != VK_SUCCESS) {
        logger.fatalc(Window::channel, "Could not create window surface: ", Vulkanic::vk_error_map[vk_result]);
    }

    // Get its framebuffer size
    int fw, fh;
    glfwGetFramebufferSize(this->glfw_window, &fw, &fh);

    // With that, create the Surface object and set it internally
    this->_surface = new Vulkanic::Surface(instance, vk_surface, { static_cast<uint32_t>(fw), static_cast<uint32_t>(fh) });

    // Do a success print
    if (logger.get_verbosity() >= Verbosity::debug) {
        switch(this->_mode) {
        case Windowing::WindowMode::windowed:
            logger.logc(Verbosity::debug, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Windowed mode.");
            break;

        case Windowing::WindowMode::fullscreen:
            logger.logc(Verbosity::debug, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in Fullscreen mode on monitor ", this->_monitor->index(), '.');
            break;

        case Windowing::WindowMode::windowed_fullscreen:
            logger.logc(Verbosity::debug, "Initialized Window '", this->_title, "' with size ", this->_extent.width, 'x', this->_extent.height, " in WindowedFullscreen mode on monitor ", this->_monitor->index(), '.');
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
    glfw_window(other.glfw_window),
    glfw_video_mode(other.glfw_video_mode)
{
    other.glfw_window = nullptr;
}

/* Destructor for the Window class. */
Window::~Window() {
    if (this->glfw_window != nullptr) {
        glfwDestroyWindow(this->glfw_window);
    }
}



/* Sets the monitor of the Window. Only relevant when the Window is not in windowed mode. */
void Window::set_monitor(const Windowing::Monitor* new_monitor) {
    // Make sure the Window is in the correct format
    if (dynamic_cast<const GLFW::Monitor*>(new_monitor) == nullptr) {
        logger.fatalc(Window::channel, "GLFW Windows only accept GLFW Monitors.");
    }

    // Otherwise, skip if the window mode is windowed
    if (this->_mode == Windowing::WindowMode::windowed) { return; }

    // But if we passed the checks, update by copying
    const GLFW::Monitor* copy = new GLFW::Monitor(*((const GLFW::Monitor*) new_monitor));
    delete this->_monitor;
    this->_monitor = (const Windowing::Monitor*) copy;
}

/* Sets the title of the Window. */
void Window::set_title(const std::string& new_title) {
    this->_title = new_title;
    glfwSetWindowTitle(this->glfw_window, this->_title.c_str());
}

/* Resizes the Window to a new extent. */
void Window::set_extent(const VkExtent2D& new_extent) {
    // Set the window size
    this->_extent = new_extent;
    glfwSetWindowSize(this->glfw_window, static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height));

    // Re-create the window surface
    VkResult vk_result;
    VkSurfaceKHR vk_surface;
    if((vk_result = glfwCreateWindowSurface(this->_surface->instance, this->glfw_window, nullptr, &vk_surface)) != VK_SUCCESS) {
        logger.fatalc(Window::channel, "Could not re-create window surface on resize: ", Vulkanic::vk_error_map[vk_result]);
    }

    // Get its framebuffer size
    int fw, fh;
    glfwGetFramebufferSize(this->glfw_window, &fw, &fh);

    // Re-create the internal Surface too
    this->_surface->recreate(vk_surface, { static_cast<uint32_t>(fw), static_cast<uint32_t>(fh) });
}

/* Changes the Window mode to the given one. Since the window will most certainly change size, you should give that as well (although it will be ignored if switching to windowed fullscreen). The monitor, in turn, is ignored when switching to windowed. */
void Window::set_mode(Windowing::WindowMode new_mode, const VkExtent2D new_extent, const Windowing::Monitor* new_monitor) {
    // Set the new_extent as our own new one
    this->_extent = new_extent;

    // Switch depending on where we come from
    switch(this->_mode) {
    case Windowing::WindowMode::windowed:
        if (new_mode == Windowing::WindowMode::windowed) { break; }

        // If we're going to windowed fullscreen, set the videmode hints first
        if (new_mode == Windowing::WindowMode::windowed_fullscreen) {
            // Overwrite the internal size with the one given by the video mode
            this->_extent.width  = static_cast<uint32_t>(this->glfw_video_mode->width);
            this->_extent.height = static_cast<uint32_t>(this->glfw_video_mode->height);

            // Set the video mode for this window first
            glfwWindowHint(GLFW_RED_BITS, this->glfw_video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, this->glfw_video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, this->glfw_video_mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, this->glfw_video_mode->refreshRate);
        }

        // Set the Window's size
        glfwSetWindowSize(this->glfw_window, static_cast<int>(this->_extent.width), static_cast<int>(this->_extent.height));

        // Now set the monitor to apply the change
        

    }
}



/* Swap operator for the Window class. */
void GLFW::swap(Window& w1, Window& w2) {
    using std::swap;

    swap((Windowing::Window&) w1, (Windowing::Window&) w2);
    swap(w1.glfw_window, w2.glfw_window);
    swap(w1.glfw_video_mode, w2.glfw_video_mode);
}
