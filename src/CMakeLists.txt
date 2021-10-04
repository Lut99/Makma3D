# Specify the libraries in this directory
add_library(Test ${CMAKE_CURRENT_SOURCE_DIR}/Test.cpp)

# Set the dependencies for this library:
target_include_directories(Test PRIVATE
                           "${INCLUDE_DIRS}")

# Add it to the list of includes & linked libraries
list(APPEND Makma3D_LIBRARIES Test)

# Carry the list to the parent scope
set(Makma3D_LIBRARIES "${Makma3D_LIBRARIES}" PARENT_SCOPE)
