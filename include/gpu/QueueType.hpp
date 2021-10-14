/* QUEUE TYPE.hpp
 *   by Lut99
 *
 * Created:
 *   14/10/2021, 11:47:31
 * Last edited:
 *   14/10/2021, 11:47:31
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains an enum that lists the possible queue type operations we want
 *   to do in the Makma3D engine. Is nested in the Vulkanic namespace,
 *   since it's mostly used in the background.
**/

#ifndef VULKANIC_QUEUE_TYPE_HPP
#define VULKANIC_QUEUE_TYPE_HPP

#include <string>

namespace Makma3D::Vulkanic {
    /* Lists the possible queue type or queue operations that we want to do in the engine. */
    enum class QueueType {
        /* Memory operations, such as copying buffers or images around. */
        memory = 0,
        /* Compute operations, such as computing physics on the GPU. */
        compute = 1,
        /* Graphics operations, such as rendering a scene on the GPU. */
        graphics = 2,
        /* Present operations, such as pushing an image to the framebuffer. */
        present = 3,
        
        /* Meta type for when none is specified. */
        undefined = 4
    };

    /* The number of different queue types relevant to the engine. */
    static constexpr const uint32_t n_queue_types = 4;

    /* Names for the GPUFeatureFlags enum. */
    static const std::string queue_type_names[] = {
        "memory",
        "compute",
        "graphics",
        "present",

        "undefined"
    };
}

#endif
