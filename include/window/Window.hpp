/* WINDOW.hpp
 *   by Lut99
 *
 * Created:
 *   06/10/2021, 15:57:54
 * Last edited:
 *   06/10/2021, 15:57:54
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Window class, which represents a single Window. Is based
 *   on some backend API (probably GLFW), and provides interaction in terms
 *   of both rendering and user input.
**/

#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <vulkan/vulkan.h>

#include "instance/Instance.hpp"
#include "vulkanic/surface/Surface.hpp"
// #include "vulkanic/gpu/GPU.hpp"

#include "WindowMode.hpp"
#include "Monitor.hpp"

namespace Makma3D::Windowing {
    /* Forward declaration of the Instance class. */
    extern class Makma3D::Instance;



    /* The Window class, which represents a single, renderable Window. Is technically a purely virtual class, as window APIs implement their own derivative. */
    class Window {
    public:
        /* Channel name for the Window class. */
        static constexpr const char* channel = "Window";

        /* The Instance to which this Window is bound. */
        const Makma3D::Instance& instance;

    protected:
        /* The Window itself. */
        GLFWwindow* glfw_window;

        /* The monitor where the Window lives, if relevant. Is set to nullptr otherwise. */
        const Windowing::Monitor* _monitor;

        /* The title of the Window. */
        std::string _title;
        /* The size of the Window. */
        VkExtent2D _extent;
        /* The WindowMode where the Window is in. */
        WindowMode _mode;

        /* The Surface object used to create a Swapchain with. */
        Vulkanic::Surface* _surface;
        /* The Swapchain object that we wrap. */
        /* TBD */


        /* Returns the nearest monitor to the current Window position. Only called if the current mode is windowed. */
        const Windowing::Monitor* _find_nearest_monitor() const;
        /* Private helper function that reconstructs the Surface. */
        void _reconstruct_surface();
        /* Private helper function that reconstructs the Swapchain. */
        /* TBD */

    public:
        /* Constructor for the Window class.
         * @param instance Instance to base the internal Vulkan structures on. Will also be used to select the nearest monitor in case we're going fullscreen from windowed.
         * @param monitor The Monitor where the Window will be spawned. Will be ignored if the WindowMode is set to windowed.
         * @param title String title for the Window.
         * @param extent The desired size (in pixels) of the Window. Will be ignored if the WindowMode is set to windowed fullscreen.
         * @param mode The WindowMode for the Window.
         */
        Window(const Makma3D::Instance& instance, const Windowing::Monitor* monitor, const std::string& title, const VkExtent2D& extent, Windowing::WindowMode mode);
        /* Copy constructor for the Window class, which is deleted. */
        Window(const Window& other) = delete;
        /* Move constructor for the Window class. */
        Window(Window&& other);
        /* Destructor for the Window class. */
        ~Window();

        // /* Uses the given GPU to create the internal swapchain. Must be called before the window can be rendered to, obviously. */
        // void bind(const Vulkanic::GPU& gpu);

        /* Does a single pass of the window events for this window. Returns whether the window should stay open (true) or not (false). */
        bool loop() const;

        /* Sets the monitor of the Window, giving it a new size while at it. Only relevant when the Window is not in windowed mode (does nothing if it is).
         * @param new_monitor The new Monitor of the Window.
         * @param new_extent The new size of the Window on its new monitor. Ignored if the window is in windowed fullscreen mode.
         */
        void set_monitor(const Windowing::Monitor* new_monitor, const VkExtent2D& new_extent);
        /* Sets the title of the Window.
         * @param new_title The new title of the Window.
         */
        void set_title(const std::string& new_title);
        /* Resizes the Window to a new extent. Does nothing if the Window is in windowed fullscreen mode.
         * @param new_extent The new size of the Window.
         */
        void set_extent(const VkExtent2D& new_extent);
        /* Changes the Window mode to the given one.
         * @param new_mode The new WindowMode to set the Window to. If this is the same as the old mode, then the Window does nothing.
         * @param new_extent The new size of the Window. Ignored if the new mode is windowed fullscreen.
         * @param new_monitor The new monitor of the Window. Use nullptr to use the window already here, or to find the closest monitor in case we're coming from windowed mode. Ignored if we're going to windowed mode.
         */
        void set_mode(WindowMode new_mode, const VkExtent2D new_extent, const Windowing::Monitor* new_monitor);

        /* Returns the monitor to which the Window is bound, if relevant (i.e., not windowed). Returns nullptr otherwise. */
        inline const Windowing::Monitor* monitor() const { return this->_monitor; }
        /* Returns the title of the Window. */
        inline const std::string& title() const { return this->_title; }
        /* Returns the size (extent) of the Window as a VkExtent2D struct. */
        inline const VkExtent2D& extent() const { return this->_extent; }
        /* Returns the current window mode of the Window. */
        inline WindowMode mode() const { return this->_mode; }

        /* Returns a reference to the internal surface, which can coincidentally be used for an accurate size of the framebuffer. */
        inline const Vulkanic::Surface& surface() const { return *this->_surface; }

        /* Copy assignment operator for the Window class, which is deleted. */
        Window& operator=(const Window& other) = delete;
        /* Move assignment operator for the Window class. */
        inline Window& operator=(Window&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the Window class. */
        friend void swap(Window& w1, Window& w2);

    };

    /* Swap operator for the Window class. */
    void swap(Window& w1, Window& w2);

}

#endif
