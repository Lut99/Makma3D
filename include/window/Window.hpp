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

#include "vulkanic/surface/Surface.hpp"

namespace Makma3D {
    /* The Window class, which represents a single, renderable Window. Is technically a purely virtual class, as window APIs implement their own derivative. */
    class Window {
    public:
        /* Channel name for the Window class. */
        static constexpr const char* channel = "Window";

    protected:
        /* The Surface object used to create a Swapchain with. */
        Vulkanic::Surface surface;
        /* The Swapchain object that we wrap. */


        /* Protected constructor for the Window class, which takes a GPU and a VkSurfaceKHR to initialize its surface and then its swapchain with. */
        Window(/* TBD */ const Vulkanic::Surface& surface);

    public:
        /* Copy constructor for the Window class, which is deleted. */
        Window(const Window& other) = delete;
        /* Move constructor for the Window class. */
        Window(Window&& other);
        /* Destructor for the Window class. */
        ~Window();

        

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
