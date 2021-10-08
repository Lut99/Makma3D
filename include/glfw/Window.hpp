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

#include "Monitor.hpp"

namespace Makma3D::GLFW {
    /* The GLFW::Window class, which extends from the base Window class and implements it with the GLFW3 API. */
    class Window: public Windowing::Window {
    public:
        /* Channel name for the Window class. */
        static constexpr const char* channel = "GLFWWindow";

    private:
        /* The GLFWWindow we wrap around. */
        GLFWwindow* glfw_window;
        /* The video mode from the screen from before the window was created. */
        const GLFWvidmode* glfw_video_mode;
    
    public:
        /* Constructor for the Window class.
         * @param instance The Vulkanic Instance that we use to create the Surface.
         * @param monitor The Monitor where the Window will be spawned. Will be ignored if the WindowMode is set to windowed. Otherwise, it will be copied, and so the pointer can be safely deleted as soon as the constructor returns.
         * @param title String title for the Window.
         * @param extent The desired size (in pixels) of the Window. Will be ignored if the WindowMode is set to windowed fullscreen.
         * @param mode The WindowMode for the Window.
         */
        Window(const Vulkanic::Instance& instance, const GLFW::Monitor* monitor, const std::string& title, const VkExtent2D& extent, Windowing::WindowMode mode);
        /* Copy constructor for the Window class, which is deleted. */
        Window(const Window& other) = delete;
        /* Move constructor for the Window class. */
        Window(Window&& other);
        /* Destructor for the Window class. */
        ~Window();

        /* Sets the title of the Window.
         * @param new_title The new title of the Window.
         */
        virtual void set_title(const std::string& new_title);
        /* Resizes the window to the given size. If the current mode is set to windowed fullscreen, then calling this function does nothing as the size is always that of the desktop resolution.
         * @param new_extent The new size of the Window, in pixels.
         */
        virtual void resize(const VkExtent2D& new_extent);
        /* Resizes the window by moving it to another monitor. Is pretty useless to call if the current mode is set to windowed, or if the given monitor is the same as the old one.
         * @param new_monitor The new Monitor to move the Window to.
         */
        virtual void resize(const Windowing::Monitor* new_monitor);
        /* Changes the Window mode to the given one. Since the window will most certainly change size, you should give that as well (although it will be ignored if switching to windowed fullscreen). The monitor, in turn, is ignored when switching to windowed. */
        virtual void set_mode(Windowing::WindowMode new_mode, const VkExtent2D new_extent, const Windowing::Monitor* new_monitor);

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
