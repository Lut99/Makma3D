# Add the subdirectories
add_subdirectory(window)
add_subdirectory(gpu)
add_subdirectory(vulkanic)
add_subdirectory(instance)
add_subdirectory(tools)

# Carry the list to the parent scope
set(EXTRA_LIBS "${EXTRA_LIBS}" PARENT_SCOPE)
