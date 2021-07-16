#include <stdio.h>
#include "glad/glad.h"
#include "OpenGLGraphicsManager.hpp"

using namespace Me;

extern gladGLversionStruct GLVersion;

int OpenGLGraphicsManager::Initialize() {
    int result;

    result = gladLoadGL();
    if (!result) {
        printf("OpenGL load failed!\n");
        result = -1;
    } else {
        result = 0;
        printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);

        if (GLAD_GL_VERSION_3_0) {
            // Set the depth buffer to be entirely cleared to 1.0 value
            glClearDepth(1.0f);

            // Enable depth testing
            glEnable(GL_DEPTH_TEST);

            // Enable back face culling
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
    }

    return result;
}

void OpenGLGraphicsManager::Finalize() {
    GraphicsManager::Finalize();
}

void OpenGLGraphicsManager::Tick() {
    GraphicsManager::Tick();
}
