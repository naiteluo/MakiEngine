# Cocoa application

find_library(COCOA_LIBRARY Cocoa required)

add_executable(${ME_PLATFORM_PREFIX}Cocoa MACOSX_BUNDLE
        CocoaApplication.mm
        CocoaApplicationConfig.cpp
        AppDelegate.m
        WindowDelegate.m
        )
target_link_libraries(${ME_PLATFORM_PREFIX}Cocoa
        Common
        ${COCOA_LIBRARY}
        )

add_executable(${ME_PLATFORM_PREFIX}Cocoa_OpenGL MACOSX_BUNDLE
        CocoaApplication.mm
        CocoaOpenGLApplication.mm
        CocoaOpenGLApplicationConfig.cpp
        AppDelegate.m
        WindowDelegate.m
        GLView.mm
        )
target_link_libraries(${ME_PLATFORM_PREFIX}Cocoa_OpenGL
        OpenGLRHI
        Common
        ${COCOA_LIBRARY}
        ${OPENGL_LIBRARY}
        dl
        )
target_include_directories(${ME_PLATFORM_PREFIX}Cocoa_OpenGL PRIVATE "${PROJECT_SOURCE_DIR}/External/GL/include")
me_add_xg_platform_dependencies(${ME_PLATFORM_PREFIX}Cocoa_OpenGL)