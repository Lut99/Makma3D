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
#include "window/Window.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Windowing;


/***** WINDOW CLASS *****/
/* Protected constructor for the Window class, which doesn't do anything except taking some of the window's properties. That means it's the derived class' constructor responsibility to create the Surface. */
Window::Window(const Windowing::Monitor* monitor, const std::string& title, const VkExtent2D& extent, WindowMode window_mode) :
    _monitor(monitor),

    _title(title),
    _extent(extent),
    _mode(window_mode)
{
    // Make sure the monitor is valid
    if (!this->_allowed_monitor(this->_monitor)) {
        logger.fatalc(Window::channel, "Invalid monitor for ", this->_api_name(), " backend.");
    }
}

/* Move constructor for the Window class. */
Window::Window(Window&& other) :
    _monitor(other._monitor),

    _title(other._title),
    _extent(other._extent),
    _mode(other._mode),

    _surface(other._surface)
{
    other._surface = nullptr;
}

/* Destructor for the Window class. */
Window::~Window() {
    if (this->_surface != nullptr) {
        delete this->_surface;
    }
}



/* Uses the given GPU to create the internal swapchain. Must be called before the window can be rendered to, obviously. */
void Window::bind(const Vulkanic::GPU& gpu) {

}



/* Sets the monitor of the Window, giving it a new size while at it. Only relevant when the Window is not in windowed mode (does nothing if it is). */
void Window::set_monitor(const Windowing::Monitor* new_monitor, const VkExtent2D& new_extent) {
    // If the current mode is Windowed, do nothing
    if (this->_mode == WindowMode::windowed) { return; }

    // Make sure the Monitor is valid for the backend
    if (!this->_allowed_monitor(new_monitor)) {
        logger.fatalc(Window::channel, "Invalid monitor for ", this->_api_name(), " backend.");
    }

    // Set the monitor internally
    this->_monitor = new_monitor;

    // If we're not in windowed fullscreen, set the size too
    if (this->_mode != WindowMode::windowed_fullscreen) {
        this->_extent = new_extent;
    }

    // Now run the API's monitor replacement
    this->_replace_monitor();

    // We now have to rebuild the surface & swapchain
    this->_reconstruct_surface();
    /* TBD */
}

/* Sets the title of the Window. */
void Window::set_title(const std::string& new_title) {
    // Set the title internally
    this->_title = new_title;
    // Run the backend's push
    this->_replace_title();
}

/* Resizes the Window to a new extent. Does nothing if the Window is in windowed fullscreen mode. */
void Window::set_extent(const VkExtent2D& new_extent) {
    // Set the extent internally
    this->_extent = new_extent;

    // Run the backend push
    this->_resize_window();

    // We now have to rebuild the surface & swapchain
    this->_reconstruct_surface();
    /* TBD */
}

/* Changes the Window mode to the given one. */
void Window::set_mode(WindowMode new_mode, const VkExtent2D new_extent, const Windowing::Monitor* new_monitor) {
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
        } else {
            // Make sure the monitor is valid for the backend
            if (!this->_allowed_monitor(new_monitor)) {
                logger.fatalc(Window::channel, "Invalid monitor for ", this->_api_name(), " backend.");
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
        VkOffset2D middle = this->_monitor->get_center();
        middle.x += static_cast<int32_t>(this->_extent.width / 2);
        middle.y += static_cast<int32_t>(this->_extent.height / 2);

        // Convert the window to the center of this
        this->_make_windowed(middle);

        // Remove the internal monitor
        this->_monitor = nullptr;
    } else if (new_mode == WindowMode::fullscreen) {
        // Set the new monitor
        this->_monitor = new_monitor;

        // Simply call the backend function
        this->_make_fullscreen();
    } else if (new_mode == WindowMode::windowed_fullscreen) {
        // Set the new monitor
        this->_monitor = new_monitor;

        // Simply call the backend function
        this->_make_windowed_fullscreen();
    } else {
        logger.fatalc(Window::channel, "Unsupported WindowMode '", window_mode_names[(int) new_mode], "'.");
    }

    // With the mode changed, reconstruct the surface and the swapchain
    this->_reconstruct_surface();
    /* TBD */
}



/* Swap operator for the Window class. */
void Windowing::swap(Window& w1, Window& w2) {
    using std::swap;

    swap(w1._monitor, w2._monitor);

    swap(w1._title, w2._title);
    swap(w1._extent, w2._extent);
    swap(w1._mode, w2._mode);
    
    swap(w1._surface, w2._surface);
}
