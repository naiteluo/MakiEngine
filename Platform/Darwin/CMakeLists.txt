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

# Cocoa application
find_library(COCOA_LIBRARY Cocoa required)
add_executable(DarwinHelloCocoa MACOSX_BUNDLE
        CocoaApplication.mm
        CocoaApplicationConfig.cpp
        AppDelegate.m
        WindowDelegate.m
        )
target_link_libraries(DarwinHelloCocoa
        Common
        ${COCOA_LIBRARY}
        )

add_executable(DarwinHelloCocoaOpenGL MACOSX_BUNDLE
        CocoaApplication.mm
        CocoaOpenGLApplication.mm
        CocoaOpenGLApplicationConfig.cpp
        AppDelegate.m
        WindowDelegate.m
        GLView.mm
        )
target_link_libraries(DarwinHelloCocoaOpenGL
        OpenGLRHI
        Common
        ${COCOA_LIBRARY}
        )