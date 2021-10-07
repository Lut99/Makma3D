/* GPU FEATURE FLAGS.hpp
 *   by Lut99
 *
 * Created:
 *   07/10/2021, 22:35:51
 * Last edited:
 *   07/10/2021, 22:35:51
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains an enum that lists GPU features and allows you to enable
 *   them.
**/

#ifndef VULKANIC_GPU_FEATURE_FLAGS_HPP
#define VULKANIC_GPU_FEATURE_FLAGS_HPP

#include <string>
#include <unordered_map>

namespace Makma3D::Vulkanic {
    /* Values for the GPUFeatureFlags enum. */
    namespace GPUFeatureFlagsValues {
        enum feature {
            /* Shortcut for no feature given. */
            none = 0,
            /* Shortcut for all features. */
            all = ~0,

            /* If set, then the device will enable anisotropic filtering for image samplers. */
            anisotropy = 0x1

        };
    }
    /* Allows one to select one or more GPU features to enable. */
    using GPUFeatureFlags = GPUFeatureFlagsValues::feature;

    /* Names for the GPUFeatureFlags enum. */
    static const std::unordered_map<GPUFeatureFlags, std::string> gpu_feature_flags_names = {
        { GPUFeatureFlags::all, "all" },
        { GPUFeatureFlags::none, "none" },

        { GPUFeatureFlags::anisotropy, "anisotropy" }
    };
}

#endif
