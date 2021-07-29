include_directories(/opt/X11/include)
link_directories(/opt/X11/lib)

add_executable(${ME_PLATFORM_PREFIX}X11_OpenGL_App
        XcbApplication.cpp OpenGLApplication.cpp
        "${PROJECT_SOURCE_DIR}/External/GL/src/glad_glx.c"
        )
target_link_libraries(${ME_PLATFORM_PREFIX}X11_OpenGL_App
        OpenGLRHI Common
        xcb X11 X11-xcb dl
        )
target_include_directories(${ME_PLATFORM_PREFIX}X11_OpenGL_App PRIVATE
        "${PROJECT_SOURCE_DIR}/External/GL/include"
        )