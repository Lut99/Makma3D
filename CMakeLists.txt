# CMAKELIST for the MAKMA3D-project
#   by Lut99

# If we're on Windows, prefix the GLFW path
IF(WIN32)
set(GLFW3_PATH "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
list(APPEND CMAKE_PREFIX_PATH "${GLFW3_PATH}")
ENDIF()

# Get the VULKAN & GLFW library
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Specify the C++-standard to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define all include directories
get_target_property(GLFW_DIR glfw INTERFACE_INCLUDE_DIRECTORIES)
SET(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" "${Vulkan_INCLUDE_DIRS}" "${GLFW_DIR}")

# Load the libraries
add_subdirectory(src)



##### MAKMA3D TARGET #####
# Set the target with a dummy file
add_library(makma3D ${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp)
# Add the include directories for this target
target_include_directories(makma3D PRIVATE "${INCLUDE_DIRS}")
# Add which libraries to link
target_link_libraries(makma3D PUBLIC
                      ${EXTRA_LIBS}
                      ${Vulkan_LIBRARIES}
                      glfw)



# Push the library & includes up
set(Makma3D_LIBRARIES makma3D PARENT_SCOPE)
# Same for public headers
set(Makma3D_INCLUDES ${INCLUDE_DIRS} PARENT_SCOPE)
