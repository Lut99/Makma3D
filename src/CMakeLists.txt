# Add the subdirectories
add_subdirectory(window)
add_subdirectory(glfw)
add_subdirectory(vulkanic)
add_subdirectory(instance)
add_subdirectory(tools)

# Carry the list to the parent scope
set(Makma3D_LIBRARIES "${Makma3D_LIBRARIES}" PARENT_SCOPE)
