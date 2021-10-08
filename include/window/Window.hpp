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

#include "vulkanic/surface/Surface.hpp"
#include "vulkanic/gpu/GPU.hpp"

#include "WindowMode.hpp"
#include "Monitor.hpp"

namespace Makma3D::Windowing {
    /* The Window class, which represents a single, renderable Window. Is technically a purely virtual class, as window APIs implement their own derivative. */
    class Window {
    public:
        /* Channel name for the Window class. */
        static constexpr const char* channel = "Window";

    protected:
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


        /* Protected constructor for the Window class, which doesn't do anything except taking some of the window's properties. That means it's the derived class' constructor responsibility to create the Surface. */
        Window(const std::string& title, const VkExtent2D& extent, WindowMode window_mode, const Windowing::Monitor* monitor);

    public:
        /* Copy constructor for the Window class, which is deleted. */
        Window(const Window& other) = delete;
        /* Move constructor for the Window class. */
        Window(Window&& other);
        /* Destructor for the Window class. */
        virtual ~Window();

        /* Uses the given GPU to create the internal swapchain. Must be called before the window can be rendered to, obviously. */
        void bind(const Vulkanic::GPU& gpu);

        /* Sets the monitor of the Window. Only relevant when the Window is not in windowed mode. */
        virtual void set_monitor(const Windowing::Monitor* monitor) = 0;
        /* Sets the title of the Window. */
        virtual void set_title(const std::string& new_title) = 0;
        /* Resizes the Window to a new extent. */
        virtual void set_extent(const VkExtent2D& new_extent) = 0;
        /* Changes the Window mode to the given one. Since the window will most certainly change size, you should give that as well (although it will be ignored if switching to windowed fullscreen). The monitor, in turn, is ignored when switching to windowed. */
        virtual void set_mode(WindowMode new_mode, const VkExtent2D new_extent, const Windowing::Monitor* monitor) = 0;

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
