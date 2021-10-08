/* SURFACE.hpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 18:13:32
 * Last edited:
 *   07/10/2021, 18:13:32
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Surface class, which wraps a given VkSurfaceKHR object.
 *   The object could originate from anywhere.
**/

#ifndef VULKANIC_SURFACE_HPP
#define VULKANIC_SURFACE_HPP

#include "vulkanic/instance/Instance.hpp"

namespace Makma3D::Vulkanic {
    /* The Surface class, which wraps a VkSurfaceKHR object that comes from a window library. */
    class Surface {
    public:
        /* Channel name for the Surface class. */
        static constexpr const char* channel = "VulkanicSurface";

        /* The Instance with which the Surface is instantiated. */
        const Vulkanic::Instance& instance;
    
    private:
        /* The VkSurfaceKHR object that we wrap. */
        VkSurfaceKHR vk_surface;
        /* The size (in pixels) of the surface. */
        VkExtent2D _extent;
    
    public:
        /* Constructor for the Surface class, which takes an instance with which the surface is create, said surface it's supposed to wrap and the size (in pixels) of the surface's framebuffer. The Surface will be automatically destroyed when the class is. */
        Surface(const Vulkanic::Instance& instance, const VkSurfaceKHR& vk_surface, const VkExtent2D& extent);
        /* Copy constructor for the Surface class, which is deleted as a window only has one surface at a time. */
        Surface(const Surface& other) = delete;
        /* Move constructor for the Surface class. */
        Surface(Surface&& other);
        /* Destructor for the Surface class. */
        ~Surface();

        /* "Re-creates" the Surface by destroying the internal one and replacing it with the given VkSurfaceKHR. The extent is the new size of the surface. */
        void recreate(const VkSurfaceKHR& vk_surface, const VkExtent2D& extent);

        /* Returns the size (in pixels) of framebuffers rendering to this surface. */
        inline const VkExtent2D& extent() const { return this->_extent; }
        /* Expliticly returns the internal VkSurfaceKHR object. */
        inline const VkSurfaceKHR& vk() const { return this->vk_surface; }
        /* Implicitly returns the internal VkSurfaceKHR object. */
        inline operator VkSurfaceKHR() const { return this->vk_surface; }

        /* Copy assignment operator for the Surface class, which is obviously deleted. */
        Surface& operator=(const Surface& other) = delete;
        /* Move assignment operator for the Surface class. */
        inline Surface& operator=(Surface&& other) { if (this != &other) { swap(*this, other); } return *this; }
        /* Swap operator for the Surface class. */
        friend void swap(Surface& s1, Surface& s2);

    };

    /* Swap operator for the Surface class. */
    void swap(Surface& s1, Surface& s2);

}

#endif
