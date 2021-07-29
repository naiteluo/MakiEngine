include_directories(/opt/X11/include)
link_directories(/opt/X11/lib)

# prue xcb demo
add_executable(${ME_PLATFORM_PREFIX}Pure_XCB hello_xcb.cpp)
target_link_libraries(
        ${ME_PLATFORM_PREFIX}Pure_XCB
        xcb X11 X11-xcb dl
)

# pure x11 opengl demo
add_executable(${ME_PLATFORM_PREFIX}Pure_XCB_OpenGL hello_opengl.cpp)
target_link_libraries(${ME_PLATFORM_PREFIX}Pure_XCB_OpenGL
        xcb X11 X11-xcb GL GLU dl
        )