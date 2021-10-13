/* PHYSICAL DEVICE TYPE.hpp
 *   by Lut99
 *
 * Created:
 *   12/10/2021, 16:37:41
 * Last edited:
 *   12/10/2021, 16:37:41
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains an enum that lists all possible GPU types.
**/

#ifndef VULKANIC_PHYSICAL_DEVICE_TYPE_HPP
#define VULKANIC_PHYSICAL_DEVICE_TYPE_HPP

#include <string>

namespace Makma3D::GPU {
    /* Enum that lists the possible GPU types. Very closely based on Vulkan's VkPhysicalDeviceType enum. */
    enum class PhysicalDeviceType {
        /* Meta type for when nothing is specified. */
        undefined = 0,

        /* A device that is simply the processor itself. */
        cpu = 1,
        /* An integrated GPU in the host processor (think Intel integrated or AMD APU's). */
        integrated = 2,
        /* A dedicated GPU, i.e., a separate chip or card in the computer. */
        discrete = 3,
        /* A virtual GPU, which could be a group of GPUs or something else that pretends to be one. */
        simulated = 4,
        /* Device who's type is not listed in the previous types. */
        other = 5
    };

    /* Maps PhysicalDeviceType enum values to readable strings. */
    static const std::string physical_device_type_names[] = {
        "undefined",

        "CPU",
        "integrated",
        "discrete",
        "simulated",
        "other"
    };

}

#endif
