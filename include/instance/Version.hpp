/* VERSION.hpp
 *   by Lut99
 *
 * Created:
 *   12/10/2021, 17:01:41
 * Last edited:
 *   12/10/2021, 17:01:41
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Simple class that allows one to create and manage three-part version
 *   numbers.
**/

#ifndef INSTANCE_VERSION_HPP
#define INSTANCE_VERSION_HPP

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Makma3D {
    /* The Version class, which allows you to make and manage versions. */
    class Version {
    public:
        /* The major version number. */
        uint16_t major;
        /* The minor version number. */
        uint16_t minor;
        /* The patch version number. */
        uint16_t patch;
    
    public:
        /* Constructor for the Version class.
         * @param major The major version number for this Version number. Note that only the first ten bits are used, so your number can range 0-1023.
         * @param minor The minor version number for this Version number. Note that only the first ten bits are used, so your number can range 0-1023.
         * @param patch The patch version number for this Version number. Note that only the first twelve bits are used, so your number can range 0-4095.
         */
        Version(uint16_t major, uint16_t minor, uint16_t patch): major(major), minor(minor), patch(patch) {}

        /* Returns a single uint32_t version number in the way Vulkan likes. */
        inline uint32_t vk() const { return VK_MAKE_VERSION(this->major, this->minor, this->patch); }

    };
}

#endif
