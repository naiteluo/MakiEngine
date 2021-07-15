# multi-platform support
add_subdirectory(Empty)
IF (${APPLE})
    add_subdirectory(Darwin)
ENDIF (${APPLE})