/* SURFACE.cpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 18:13:36
 * Last edited:
 *   07/10/2021, 18:13:36
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Surface class, which wraps a given VkSurfaceKHR object.
 *   The object could originate from anywhere.
**/

#include "tools/Logger.hpp"
#include "vulkanic/auxillary/ErrorCodes.hpp"

#include "vulkanic/surface/Surface.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Vulkanic;


/***** SURFACE CLASS *****/
/* Constructor for the Surface class, which takes an instance with which the surface is create, said surface it's supposed to wrap and the size (in pixels) of the surface's framebuffer. The Surface will be automatically destroyed when the class is. */
Surface::Surface(const Vulkanic::Instance& instance, const VkSurfaceKHR& vk_surface, const VkExtent2D& extent) :
    instance(instance),
    vk_surface(vk_surface),
    _extent(extent)
{}

/* Move constructor for the Surface class. */
Surface::Surface(Surface&& other) :
    instance(other.instance),
    vk_surface(other.vk_surface),
    _extent(other._extent)
{
    // Mark the swapchain as non-present anymore
    this->vk_surface = nullptr;
}

/* Destructor for the Surface class. */
Surface::~Surface() {
    // Deallocate the surface if it no longer exists
    if (this->vk_surface != nullptr) {
        vkDestroySurfaceKHR(this->instance, this->vk_surface, nullptr);
    }
}



/* "Re-creates" the Surface by destroying the internal one and replacing it with the given VkSurfaceKHR. The extent is the new size of the surface. */
void Surface::recreate(const VkSurfaceKHR& vk_surface, const VkExtent2D& extent) {
    vkDestroySurfaceKHR(this->instance, this->vk_surface, nullptr);
    this->vk_surface = vk_surface;
    this->_extent = extent;
}



/* Swap operator for the Surface class. */
void Vulkanic::swap(Surface& s1, Surface& s2) {
    #ifndef NDEBUG
    // Check if the instances are actually the same
    if (&s1.instance != &s2.instance) { logger.fatalc(Surface::channel, "Cannot swap surfaces with different instances."); }
    #endif

    using std::swap;

    swap(s1.vk_surface, s2.vk_surface);
    swap(s1._extent, s2._extent);
}
