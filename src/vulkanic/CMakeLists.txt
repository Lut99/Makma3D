# Add the subdirectories
add_subdirectory(instance)

# Carry the list to the parent scope
set(EXTRA_LIBS "${EXTRA_LIBS}" PARENT_SCOPE)
