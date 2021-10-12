/* QUEUE TYPE FLAGS.hpp
 *   by Lut99
 *
 * Created:
 *   10/10/2021, 16:21:52
 * Last edited:
 *   10/10/2021, 16:21:52
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains an enum describing a queue family's capabilities.
**/

#ifndef VULKANIC_QUEUE_TYPE_FLAGS_HPP
#define VULKANIC_QUEUE_TYPE_FLAGS_HPP

#include <string>
#include <unordered_map>

namespace Makma3D::Vulkanic {
    /* Values for the QueueTypeFlags enum. */
    namespace QueueTypeFlagsValues {
        enum type {
            /* Meta enum value for nothing supported. */
            none = 0,
            /* Meta enum value for everything supported. */
            all = ~0,

            /* This queue supports memory operations. */
            memory = 0x1,
            /* This queue supports compute operations. */
            compute = 0x2,
            /* This queue supports graphics operations. */
            graphics = 0x4,
            /* This queue supports present operations. */
            present = 0x8
        };
    };
    /* The QueueTypeFlags enum, which is used to describe what capabilities a queue (family) has. */
    using QueueTypeFlags = QueueTypeFlagsValues::type;

    /* Defines the or-operator for the QueueTypeFlags. */
    inline QueueTypeFlags operator|(QueueTypeFlags qtf1, QueueTypeFlags qtf2) { return (QueueTypeFlags) ((uint8_t) qtf1 | (uint8_t) qtf2); }
    /* Defines the assign or-operator for the QueueTypeFlags. */
    inline QueueTypeFlags& operator|=(QueueTypeFlags& qtf1, QueueTypeFlags qtf2) { return (QueueTypeFlags&) ((uint8_t&) qtf1 |= (uint8_t) qtf2); }
    /* Defines the and-operator for the QueueTypeFlags. */
    inline QueueTypeFlags operator&(QueueTypeFlags qtf1, QueueTypeFlags qtf2) { return (QueueTypeFlags) ((uint8_t) qtf1 & (uint8_t) qtf2); }
    /* Defines the and-operator for the QueueTypeFlags. */
    inline QueueTypeFlags& operator&=(QueueTypeFlags& qtf1, QueueTypeFlags qtf2) { return (QueueTypeFlags&) ((uint8_t&) qtf1 &= (uint8_t) qtf2); }



    /* The number of different capabilities relevant to the engine. */
    static constexpr const uint32_t n_queue_family_types = 4;
    /* Static & constant list of the four different capabilities relevant to the engine. */
    static const QueueTypeFlags queue_family_types[n_family_types] = {
        QueueTypeFlags::memory,
        QueueTypeFlags::compute,
        QueueTypeFlags::graphics,
        QueueTypeFlags::present
    };

    /* Names for the GPUFeatureFlags enum. */
    static const std::unordered_map<QueueTypeFlags, std::string> queue_type_flags_names = {
        { QueueTypeFlags::all, "all" },
        { QueueTypeFlags::none, "none" },

        { QueueTypeFlags::memory, "memory" },
        { QueueTypeFlags::compute, "compute" },
        { QueueTypeFlags::graphics, "graphics" },
        { QueueTypeFlags::present, "present" }
    };

}

#endif
