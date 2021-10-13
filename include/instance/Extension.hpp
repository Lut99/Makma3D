/* EXTENSION.hpp
 *   by Lut99
 *
 * Created:
 *   12/10/2021, 17:09:26
 * Last edited:
 *   12/10/2021, 17:09:26
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Extension enum, which lists the possible extensions that
 *   can be enabled in the Makma3D-engine.
**/

#ifndef INSTANCE_EXTENSION_HPP
#define INSTANCE_EXTENSION_HPP

namespace Makma3D {
    /* The Extension enum, which lists the possible extensions supported by the Makma3D engine. */
    enum class Extension {
        /* Meta enum value for a non-specified extension. */
        undefined = 0,

        /* When enabled, lets the backend print extra debug information and enabled Vulkan validation layers. */
        debug = 1

    };

    /* Maps Extension enum values to readable strings. */
    static const char* extension_names[] = {
        "undefined",

        "debug"
    };
}

#endif
