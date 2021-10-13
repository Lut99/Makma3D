/* DEVICE FEATURE.hpp
 *   by Lut99
 *
 * Created:
 *   13/10/2021, 18:04:43
 * Last edited:
 *   13/10/2021, 18:04:43
 * Auto updated?
 *   Yes
 *
 * Description:
 *   List of Vulkan DeviceFeatures that may be used by the Makma3D engine.
**/

#ifndef VULKANIC_DEVICE_FEATURES_HPP
#define VULKANIC_DEVICE_FEATURES_HPP

#include <string>

namespace Makma3D::Vulkanic {
    /* Enum that defines the DeviceFeatures that may be used by the Makma3D engine. */
    enum class DeviceFeature {
        /* Meta enum value that is used in undefined cases. */
        undefined = 0,

        /* if enabled, the device supports anisotropic filtering. */
        anisotropy = 1
    };

    /* Maps DeviceFeature enum values to readable strings. */
    static const std::string device_feature_names[] = {
        "undefined",

        "anisotropy"
    };
}

#endif
