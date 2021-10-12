/* QUEUE FAMILY INFO.hpp
 *   by Lut99
 *
 * Created:
 *   10/10/2021, 16:27:37
 * Last edited:
 *   10/10/2021, 16:27:37
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the QueueFamilyInfo struct, which lists useful information
 *   about a single queue family.
**/

#ifndef VULKANIC_QUEUE_FAMILY_INFO_HPP
#define VULKANIC_QUEUE_FAMILY_INFO_HPP

#include <cstdint>

#include "QueueTypeFlags.hpp"

namespace Makma3D::Vulkanic {
    /* The QueueFamilyInfo struct, which lists useful information about a single queue family. */
    struct QueueFamilyInfo {
        /* The index of the queue family. */
        uint32_t index;
        
        /* The capabilities / types of this queue family. */
        QueueTypeFlags type;
        /* The number of queues that are in this family. */
        uint32_t n_queues;
    };
}

#endif
