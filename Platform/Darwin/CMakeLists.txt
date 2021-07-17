include_directories(/opt/X11/include)
link_directories(/opt/X11/lib)

# prue xcb demo
add_executable(DarwinHelloXCB
        hello_xcb.cpp
        )
target_link_libraries(DarwinHelloXCB
        xcb X11 X11-xcb
        dl
        )

# x11 opengl demo
add_executable(DarwinHelloOpenGL
        hello_opengl.cpp
        )
target_link_libraries(DarwinHelloOpenGL
        xcb X11 X11-xcb
        GL GLU
        dl
        )

add_executable(MyGameEngineOpenGL
        XcbApplication.cpp OpenGLApplication.cpp
        "${PROJECT_SOURCE_DIR}/External/GL/src/glad_glx.c"
        )
target_link_libraries(MyGameEngineOpenGL
        OpenGLRHI Common
        xcb X11 X11-xcb dl
        )
target_include_directories(MyGameEngineOpenGL PRIVATE
        "${PROJECT_SOURCE_DIR}/External/GL/include"
        )
