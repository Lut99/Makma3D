# Specify the libraries in this directory
add_library(Instance ${CMAKE_CURRENT_SOURCE_DIR}/Instance.cpp)

# Set the dependencies for this library:
target_include_directories(Instance PUBLIC
                           "${INCLUDE_DIRS}")

# Add it to the list of includes & linked libraries
list(APPEND Makma3D_LIBRARIES Instance)

# Carry the list to the parent scope
set(Makma3D_LIBRARIES "${Makma3D_LIBRARIES}" PARENT_SCOPE)
