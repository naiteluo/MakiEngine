include_directories(/opt/X11/include)
link_directories(/opt/X11/lib)

# prue xcb demo
add_executable(hello_xcb
        hello_xcb.cpp
        )
target_link_libraries(hello_xcb
        xcb X11 X11-xcb
        dl
        )

# x11 opengl demo
add_executable(hello_opengl
        hello_opengl.cpp
        )
target_link_libraries(hello_opengl
        xcb X11 X11-xcb
        GL GLU
        dl
        )