/* QUEUE FAMILY MAPPING.hpp
 *   by Lut99
 *
 * Created:
 *   10/10/2021, 21:07:18
 * Last edited:
 *   10/10/2021, 21:07:18
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains a struct used to tell a GPU how to map queue types to family
 *   indices, which determines which queue family is used for which type of
 *   operation.
**/

#ifndef VULKANIC_QUEUE_FAMILY_MAPPING_HPP
#define VULKANIC_QUEUE_FAMILY_MAPPING_HPP

#include "QueueTypeFlags.hpp"
#include "QueueFamilyInfo.hpp"

namespace Makma3D::Vulkanic {
    /* The QueueFamilyMapping struct, which defines which queue families to use for which operation. */
    struct QueueFamilyMapping {
        /* The list of queue families for each operation. The mapping is:
         *   - [0]: memory operations
         *   - [1]: compute operations
         *   - [2]: graphics operations
         *   - [3]: present operations
         * Alternatively, check the functions of this struct for a more abstract way of access.
         */
        QueueFamilyInfo families[Vulkanic::n_queue_family_types];


        /* Returns a muteable reference to the queue family (as a QueueFamilyInfo) for memory operations. */
        inline QueueFamilyInfo& memory() { return this->families[0]; }
        /* Returns an immuteable reference to the queue family (as a QueueFamilyInfo) for memory operations. */
        inline const QueueFamilyInfo& memory() const { return this->families[0]; }
        /* Returns a muteable reference to the queue family (as a QueueFamilyInfo) for compute operations. */
        inline QueueFamilyInfo& compute() { return this->families[1]; }
        /* Returns an immuteable reference to the queue family (as a QueueFamilyInfo) for compute operations. */
        inline const QueueFamilyInfo& compute() const { return this->families[1]; }
        /* Returns a muteable reference to the queue family (as a QueueFamilyInfo) for graphics operations. */
        inline QueueFamilyInfo& graphics() { return this->families[2]; }
        /* Returns an immuteable reference to the queue family (as a QueueFamilyInfo) for graphics operations. */
        inline const QueueFamilyInfo& graphics() const { return this->families[2]; }
        /* Returns a muteable reference to the queue family (as a QueueFamilyInfo) for present operations. */
        inline QueueFamilyInfo& present() { return this->families[3]; }
        /* Returns an immuteable reference to the queue family (as a QueueFamilyInfo) for present operations. */
        inline const QueueFamilyInfo& present() const { return this->families[3]; }
    };

}

#endif
