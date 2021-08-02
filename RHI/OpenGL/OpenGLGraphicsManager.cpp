#include <iostream>
#include <fstream>
#include <GfxConfiguration.h>
#include <AssetLoader.hpp>
#include "OpenGLGraphicsManager.hpp"
#include "IApplication.hpp"

const char VS_SHADER_SOURCE_FILE[] = "Shaders/basic.vs";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/basic.ps";

using namespace Me;
using namespace std;

extern gladGLversionStruct GLVersion;

namespace Me {
    extern AssetLoader *g_pAssetLoader;

    static void OutputShaderErrorMessage(unsigned int shaderId, const char *shaderFilename) {
        int logSize, i;
        char *infoLog;
        ofstream fout;

        // Get the size of the string containing the information log for the failed shader compilation message.
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

        // Increment the size by one to handle also the null terminator.
        logSize++;

        // Create a char buffer to hold the info log.
        infoLog = new char[logSize];
        if (!infoLog) {
            return;
        }

        // Now retrieve the info log.
        glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

        // Open a file to write the error message to.
        fout.open("shader-error.txt");

        // Write out the error message.
        for (i = 0; i < logSize; i++) {
            fout << infoLog[i];
        }

        // Close the file.
        fout.close();

        // Pop a message up on the screen to notify the user to check the text file for compile errors.
        cerr << "Error compiling shader.  Check shader-error.txt for message." << shaderFilename << endl;

        return;
    }

    static void OutputLinkerErrorMessage(unsigned int programId) {
        int logSize, i;
        char *infoLog;
        ofstream fout;


        // Get the size of the string containing the information log for the failed shader compilation message.
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

        // Increment the size by one to handle also the null terminator.
        logSize++;

        // Create a char buffer to hold the info log.
        infoLog = new char[logSize];
        if (!infoLog) {
            return;
        }

        // Now retrieve the info log.
        glGetProgramInfoLog(programId, logSize, NULL, infoLog);

        // Open a file to write the error message to.
        fout.open("linker-error.txt");

        // Write out the error message.
        for (i = 0; i < logSize; i++) {
            fout << infoLog[i];
        }

        // Close the file.
        fout.close();

        // Pop a message up on the screen to notify the user to check the text file for linker errors.
        cerr << "Error compiling linker.  Check linker-error.txt for message." << endl;
    }
}

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
            // Set the depth buffer to be entirely cleared to 1.0 values.
            glClearDepth(1.0f);

            // Enable depth testing.
            glEnable(GL_DEPTH_TEST);

            // Set the polygon winding to front facing for the right handed system.
            glFrontFace(GL_CW);

            // Enable back face culling.
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            // Initialize the world/model matrix to the identity matrix.
            BuildIdentityMatrix(m_worldMatrix);

            // Set the field of view and screen aspect ratio.
            float fieldOfView = PI / 4.0f;
            const GfxConfiguration &conf = g_pApp->GetConfiguration();

            float screenAspect = (float) conf.screenWidth / (float) conf.screenHeight;

            // Build the perspective projection matrix.
            BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
        }

        InitializeShader(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
        InitializeBuffers();
    }

    return result;
}

void OpenGLGraphicsManager::Finalize() {
    for (auto i = 0; i < m_Buffers.size(); ++i) {
        glDisableVertexAttribArray(i);
    }

    for (auto buf: m_Buffers) {
        if (buf.first == "index") {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glDeleteBuffers(1, &buf.second);
    }

    glDetachShader(m_shaderProgram, m_vertexShader);
    glDetachShader(m_shaderProgram, m_fragmentShader);

    // Detach the vertex and fragment shaders from the program.
    glDetachShader(m_shaderProgram, m_vertexShader);
    glDetachShader(m_shaderProgram, m_fragmentShader);

    // Delete the vertex and fragment shaders.
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    // Delete the shader program.
    glDeleteProgram(m_shaderProgram);
}

void OpenGLGraphicsManager::Tick() {
}

void OpenGLGraphicsManager::Clear() {
    // Set the color to clear the screen to.
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    // Clear the screen and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicsManager::Draw() {

}

bool OpenGLGraphicsManager::SetShaderParameters(float *worldMatrix, float *viewMatrix, float *projectionMatrix) {
    unsigned int location;

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "worldMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, worldMatrix);

    // Set the view matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "viewMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, viewMatrix);

    // Set the projection matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "projectionMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, projectionMatrix);

    return true;
}

void OpenGLGraphicsManager::InitializeBuffers() {

}

void OpenGLGraphicsManager::RenderBuffers() {

}

bool OpenGLGraphicsManager::InitializeShader(const char *vsFilename, const char *fsFilename) {
    return false;
}

void OpenGLGraphicsManager::CalculateCameraPosition() {

}
