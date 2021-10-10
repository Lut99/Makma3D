# Specify the libraries in this directory
add_library(VulkanicGPU ${CMAKE_CURRENT_SOURCE_DIR}/HardwareGPU.cpp) #${CMAKE_CURRENT_SOURCE_DIR}/GPU.cpp)

# Set the dependencies for this library:
target_include_directories(VulkanicGPU PUBLIC "${INCLUDE_DIRS}")

# Add it to the list of includes & linked libraries
list(APPEND EXTRA_LIBS VulkanicGPU)

# Carry the list to the parent scope
set(EXTRA_LIBS "${EXTRA_LIBS}" PARENT_SCOPE)
