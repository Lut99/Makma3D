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

#include "window/Window.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Windowing;


/***** WINDOW CLASS *****/
/* Protected constructor for the Window class, which doesn't do anything except taking some of the window's properties. That means it's the derived class' constructor responsibility to create the Surface. */
Window::Window(const std::string& title, const VkExtent2D& extent, WindowMode window_mode, const Windowing::Monitor* monitor) :
    _monitor(monitor),

    _title(title),
    _extent(extent),
    _mode(window_mode)
{}

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



/* Swap operator for the Window class. */
void Windowing::swap(Window& w1, Window& w2) {
    using std::swap;

    swap(w1._monitor, w2._monitor);

    swap(w1._title, w2._title);
    swap(w1._extent, w2._extent);
    swap(w1._mode, w2._mode);
    
    swap(w1._surface, w2._surface);
}
