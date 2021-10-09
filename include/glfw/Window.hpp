/* WINDOW.hpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:23:53
 * Last edited:
 *   08/10/2021, 14:23:53
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the GLFW Window derivative, that uses the GLFW library to
 *   create a window and a surface, after which the normal Window class can
 *   build a swapchain around it.
**/

#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <limits>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkanic/instance/Instance.hpp"

#include "window/WindowMode.hpp"
#include "window/Window.hpp"

#include "Instance.hpp"
#include "Monitor.hpp"

namespace Makma3D::GLFW {
    /* The GLFW::Window class, which extends from the base Window class and implements it with the GLFW3 API. */
    class Window: public Windowing::Window {
    public:
        /* Channel name for the Window class. */
        static constexpr const char* channel = "GLFWWindow";

        /* Reference to the GLFW Instance. */
        const GLFW::Instance& instance;

    private:
        /* The GLFWWindow we wrap around. */
        GLFWwindow* glfw_window;


        /* Returns the backend API's name. */
        virtual const char* _api_name() const;
        /* Checks if the given monitor makes sense for this API backend.
         * @param monitor The monitor to check.
         */
        virtual bool _allowed_monitor(const Windowing::Monitor* monitor) const;
        /* Returns the nearest monitor to the current Window position. Only called if the current mode is windowed. */
        virtual const Windowing::Monitor* _find_nearest_monitor() const;

        /* Replace the backend monitor with the internal one. The internal WindowMode is already set properly at this point. */
        virtual void _replace_monitor();
        /* Replace the title of the backend monitor with the internal one. */
        virtual void _replace_title();
        /* Resize the internal Window to the internal extent. */
        virtual void _resize_window();

        /* Sets the Window to the windowed mode. Guaranteed to not be called in the case the Window is already windowed. The new window size is already set internally. The internal Monitor is not yet destroyed at this point, but will be after this call returns.
         * @param new_pos The new position of the Window on the virtual screen as a VkOffset2D.
         */
        virtual void _make_windowed(const VkOffset2D& new_pos);
        /* Sets the Window to the fullscreen mode. Guaranteed to not be called in case the Window is already in fullscreen. The new window size is already set internally, just like the new Monitor. */
        virtual void _make_fullscreen();
        /* Sets the Window to the windowed fullscreen mode. Guaranteed to not be called in case the Window is already in fullscreen. The new monitor is already set internally, and the rest of the properties we copy from that. */
        virtual void _make_windowed_fullscreen();

        /* Reconstruct the internal Surface from the internal Window. */
        virtual void _reconstruct_surface();
    
    public:
        /* Constructor for the Window class.
         * @param glfw_instance The GLFW Instance that we use to find the nearest monitor to a windowed window.
         * @param vulkanic_instance The Vulkanic Instance that we use to create the Surface.
         * @param monitor The Monitor where the Window will be spawned. Will be ignored if the WindowMode is set to windowed. Otherwise, it will be copied, and so the pointer can be safely deleted as soon as the constructor returns.
         * @param title String title for the Window.
         * @param extent The desired size (in pixels) of the Window. Will be ignored if the WindowMode is set to windowed fullscreen.
         * @param mode The WindowMode for the Window.
         */
        Window(const GLFW::Instance& glfw_instance, const Vulkanic::Instance& vulkanic_instance, const GLFW::Monitor* monitor, const std::string& title, const VkExtent2D& extent, Windowing::WindowMode mode);
        /* Copy constructor for the Window class, which is deleted. */
        Window(const Window& other) = delete;
        /* Move constructor for the Window class. */
        Window(Window&& other);
        /* Destructor for the Window class. */
        ~Window();

        /* Does a single pass of the window events for this window. Returns whether the window should stay open (true) or not (false). */
        virtual bool loop() const;

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
