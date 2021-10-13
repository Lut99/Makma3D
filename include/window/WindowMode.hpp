/* WINDOW MODE.hpp
 *   by Lut99
 *
 * Created:
 *   08/10/2021, 14:42:48
 * Last edited:
 *   08/10/2021, 14:42:48
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains an enum that list possible Window modes: windowed,
 *   fullscreen, or windowed fullscreen.
**/

#ifndef WINDOW_WINDOW_MODE_HPP
#define WINDOW_WINDOW_MODE_HPP

#include <string>

namespace Makma3D {
    /* The WindowMode enum, which determines the mode the window is in. */
    enum class WindowMode {
        /* Meta enum value for when no mode is defined. */
        undefined = 0,

        /* A Windowed window that cannot be resized. */
        windowed = 1,
        /* A Windowed window that _can_ be resized. Is otherwise completely identical. */
        windowed_resizeable = 2,
        /* A Fullscreen window. */
        fullscreen = 3,
        /* A Windowed Fullscreen window. */
        windowed_fullscreen = 4
    };

    /* Maps WindowMode enum values to readable strings. */
    static const std::string window_mode_names[] = {
        "undefined",

        "windowed",
        "windowed resizeable",
        "fullscreen",
        "windowed fullscreen"
    };

}

#endif
