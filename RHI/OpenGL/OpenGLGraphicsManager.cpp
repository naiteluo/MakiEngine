#include <iostream>
#include <fstream>
#include "GfxConfiguration.h"
#include "AssetLoader.hpp"
#include "OpenGLGraphicsManager.hpp"
#include "IApplication.hpp"
#include "SceneManager.hpp"

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

            // Set the polygon winding to front facing for the right-handed system.
            // todo GL_CW v.s. GL_CCW
            glFrontFace(GL_CCW);

            // Enable back face culling.
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            BuildIdentityMatrix(m_DrawFrameContext.m_worldMatrix);
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

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    glDeleteProgram(m_shaderProgram);
}

void OpenGLGraphicsManager::Tick() {
}

void OpenGLGraphicsManager::Clear() {
    // Set the color to clear the screen to.
    glClearColor(0.8f, 0.3f, 0.4f, 1.0f);
    // Clear the screen and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicsManager::Draw() {
    RenderBuffers();
    glFlush();
}

bool OpenGLGraphicsManager::SetPerFrameShaderParameters() {
    unsigned int location;

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "worldMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, m_DrawFrameContext.m_worldMatrix);

    // Set the view matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "viewMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, m_DrawFrameContext.m_viewMatrix);

    // Set the projection matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "projectionMatrix");
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, m_DrawFrameContext.m_projectionMatrix);

    // Set the lighting parameters for ps shader
    location = glGetUniformLocation(m_shaderProgram, "lightPosition");
    if (location == -1) {
        return false;
    }
    glUniform3fv(location, 1, m_DrawFrameContext.m_lightPosition);

    location = glGetUniformLocation(m_shaderProgram, "lightColor");
    if (location == -1) {
        return false;
    }
    glUniform4fv(location, 1, m_DrawFrameContext.m_lightColor);

    return true;
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const char *paramName, float *param) {
    unsigned int location;

    location = glGetUniformLocation(m_shaderProgram, paramName);
    if (location == -1) {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, param);
    return true;
}

void OpenGLGraphicsManager::InitializeBuffers() {
    auto &scene = g_pSceneManager->GetSceneForRendering();
    auto pGeometryNode = scene.GetFirstGeometryNode();
    while (pGeometryNode) {
        auto pGeometry = scene.GetGeometry(pGeometryNode->GetSceneObjectRef());
        auto pMesh = pGeometry->GetMesh().lock();
        if (!pMesh) return;

        auto vertexPropertiesCount = pMesh->GetVertexPropertiesCount();
        auto vertexCount = pMesh->GetVertexCount();

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer_id;
        // generate a buffer per property
        for (int32_t i = 0; i < vertexPropertiesCount; ++i) {
            const SceneObjectVertexArray &v_property_array = pMesh->GetVertexPropertyArray(i);
            auto v_property_array_data_size = v_property_array.GetDataSize();
            auto v_property_array_data = v_property_array.GetData();

            glGenBuffers(1, &buffer_id);

            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            glBufferData(GL_ARRAY_BUFFER, v_property_array_data_size, v_property_array_data, GL_STATIC_DRAW);

            glEnableVertexAttribArray(i);
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            switch (v_property_array.GetDataType()) {
                case VertexDataType::kVertexDataTypeFloat1:
                    glVertexAttribPointer(i, 1, GL_FLOAT, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeFloat2:
                    glVertexAttribPointer(i, 2, GL_FLOAT, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeFloat3:
                    glVertexAttribPointer(i, 3, GL_FLOAT, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeFloat4:
                    glVertexAttribPointer(i, 4, GL_FLOAT, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeDouble1:
                    glVertexAttribPointer(i, 1, GL_DOUBLE, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeDouble2:
                    glVertexAttribPointer(i, 2, GL_DOUBLE, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeDouble3:
                    glVertexAttribPointer(i, 3, GL_DOUBLE, false, 0, 0);
                    break;
                case VertexDataType::kVertexDataTypeDouble4:
                    glVertexAttribPointer(i, 4, GL_DOUBLE, false, 0, 0);
                    break;
                default:
                    assert(0);
            }
            m_Buffers[v_property_array.GetAttributeName()] = buffer_id;
        }

        // generate index buffer's id
        glGenBuffers(1, &buffer_id);

        const SceneObjectIndexArray &index_array = pMesh->GetIndexArray(0);
        auto index_array_size = index_array.GetDataSize();
        auto index_array_data = index_array.GetData();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array_size, index_array_data, GL_STATIC_DRAW);

        GLsizei indexCount = static_cast<GLsizei>(index_array.GetIndexCount());
        GLenum mode;

        switch (pMesh->GetPrimitiveType()) {
            case PrimitiveType::kPrimitiveTypePointList:
                mode = GL_POINTS;
                break;
            case PrimitiveType::kPrimitiveTypeLineList:
                mode = GL_LINES;
                break;
            case PrimitiveType::kPrimitiveTypeLineStrip:
                mode = GL_LINE_STRIP;
                break;
            case PrimitiveType::kPrimitiveTypeTriList:
                mode = GL_TRIANGLES;
                break;
            case PrimitiveType::kPrimitiveTypeTriStrip:
                mode = GL_TRIANGLE_STRIP;
                break;
            case PrimitiveType::kPrimitiveTypeTriFan:
                mode = GL_TRIANGLE_FAN;
                break;
            default:
                continue;
        }

        GLenum type;
        switch (index_array.GetIndexType()) {
            case IndexDataType::kIndexDataTypeInt8:
                type = GL_UNSIGNED_BYTE;
                break;
            case IndexDataType::kIndexDataTypeInt16:
                type = GL_UNSIGNED_SHORT;
                break;
            case IndexDataType::kIndexDataTypeInt32:
                type = GL_UNSIGNED_INT;
                break;
            default:
                cerr << "Error: Unsupported Index Type " << index_array << endl;
                cerr << "Mesh: " << *pMesh << endl;
                cerr << "Geometry: " << *pGeometry << endl;
                continue;
        }

        m_Buffers["index"] = buffer_id;

        DrawBatchContext &dbc = *(new DrawBatchContext);
        dbc.vao = vao;
        dbc.mode = mode;
        dbc.type = type;
        dbc.count = indexCount;
        dbc.transform = pGeometryNode->GetCalculatedTransform();
        m_VAO.push_back(std::move(dbc));

        pGeometryNode = scene.GetNextGeometryNode();
    }
}

void OpenGLGraphicsManager::RenderBuffers() {
    static float rotateAngle = 0.0f;

    // Update world matrix to rotate the model
    rotateAngle += PI / 180;
    Matrix4X4f rotationMatrixY;
    Matrix4X4f rotationMatrixZ;
    MatrixRotationY(rotationMatrixY, 0);
    MatrixRotationZ(rotationMatrixZ, rotateAngle);
    MatrixMultiply(m_DrawFrameContext.m_worldMatrix, rotationMatrixZ, rotationMatrixY);

    // Generate the view matrix based on the camera's position.
    CalculateCamera();
    CalculateLights();

    SetPerFrameShaderParameters();

    for (auto dbc : m_VAO) {
        // Set the color shader as the current shader program and set the matrices that it will use for rendering
        glUseProgram(m_shaderProgram);
        SetPerBatchShaderParameters("objectLocalMatrix", *dbc.transform);
        glBindVertexArray(dbc.vao);
        glDrawElements(dbc.mode, dbc.count, dbc.type, 0);
    }
}

bool OpenGLGraphicsManager::InitializeShader(const char *vsFilename, const char *fsFilename) {
    std::string vertexShaderBuffer;
    std::string fragmentShaderBuffer;
    int status;

    // Load the vertex shader source file into a text buffer.
    vertexShaderBuffer = g_pAssetLoader->SyncOpenAndReadTextFileToString(vsFilename);
    if (vertexShaderBuffer.empty()) {
        return false;
    }

    // Load the fragment shader source file into a text buffer.
    fragmentShaderBuffer = g_pAssetLoader->SyncOpenAndReadTextFileToString(fsFilename);
    if (fragmentShaderBuffer.empty()) {
        return false;
    }

    // Create a vertex and fragment shader object.
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Copy the shader source code strings into the vertex and fragment shader objects.
    const char *_v_c_str = vertexShaderBuffer.c_str();
    glShaderSource(m_vertexShader, 1, &_v_c_str, NULL);
    const char *_f_c_str = fragmentShaderBuffer.c_str();
    glShaderSource(m_fragmentShader, 1, &_f_c_str, NULL);

    // Compile the shaders.
    glCompileShader(m_vertexShader);
    glCompileShader(m_fragmentShader);

    // Check to see if the vertex shader compiled successfully.
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
    if (status != 1) {
        // If it did not compile then write the syntax error message out to a text file for review.
        OutputShaderErrorMessage(m_vertexShader, vsFilename);
        return false;
    }

    // Check to see if the fragment shader compiled successfully.
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != 1) {
        // If it did not compile then write the syntax error message out to a text file for review.
        OutputShaderErrorMessage(m_fragmentShader, fsFilename);
        return false;
    }

    // Create a shader program object.
    m_shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shader to the program object.
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);

    // Bind the shader input variables.
    glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
    glBindAttribLocation(m_shaderProgram, 1, "inputNormal");

    // Link the shader program.
    glLinkProgram(m_shaderProgram);

    // Check the status of the link.
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (status != 1) {
        // If it did not link then write the syntax error message out to a text file for review.
        OutputLinkerErrorMessage(m_shaderProgram);
        return false;
    }

    return true;
}

//void OpenGLGraphicsManager::CalculateCameraPosition() {
//    Vector3f up, position, lookAt;
//    float yaw, pitch, roll;
//    Matrix4X4f rotationMatrix;
//
//
//    // Setup the vector that points upwards.
//    up.x = 0.0f;
//    up.y = 1.0f;
//    up.z = 0.0f;
//
//    // Setup the position of the camera in the world.
//    position.x = m_positionX;
//    position.y = m_positionY;
//    position.z = m_positionZ;
//
//    // Setup where the camera is looking by default.
//    lookAt.x = 0.0f;
//    lookAt.y = 0.0f;
//    lookAt.z = 1.0f;
//
//    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
//    pitch = m_rotationX * 0.0174532925f;
//    yaw = m_rotationY * 0.0174532925f;
//    roll = m_rotationZ * 0.0174532925f;
//
//    // Create the rotation matrix from the yaw, pitch, and roll values.
//    MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);
//
//    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
//    TransformCoord(lookAt, rotationMatrix);
//    TransformCoord(up, rotationMatrix);
//
//    // Translate the rotated camera position to the location of the viewer.
//    lookAt.x = position.x + lookAt.x;
//    lookAt.y = position.y + lookAt.y;
//    lookAt.z = position.z + lookAt.z;
//
//    // Finally create the view matrix from the three updated vectors.
//    BuildViewMatrix(m_viewMatrix, position, lookAt, up);
//}

void OpenGLGraphicsManager::CalculateCamera() {
    auto &scene = g_pSceneManager->GetSceneForRendering();
    auto pCameraNode = scene.GetFirstCameraNode();
    // set false to debug
    static bool log_once = true;
    if (pCameraNode) {
        Matrix4X4f rotateX;
        Matrix4X4f tmp;
        m_DrawFrameContext.m_viewMatrix = *pCameraNode->GetCalculatedTransform();
        if (!log_once) {
            cout << "camera transform (origin): " << m_DrawFrameContext.m_viewMatrix << endl;
        }
        MatrixRotationX(rotateX, -PI / 2.0f);
        Transpose(tmp, m_DrawFrameContext.m_viewMatrix);
        tmp = tmp * rotateX;
        Transpose(m_DrawFrameContext.m_viewMatrix, tmp);
        if (!log_once) {
            cout << "rotate matrix: " << rotateX << endl;
            cout << "camera transform: " << m_DrawFrameContext.m_viewMatrix << endl;
            log_once = true;
        }
        InverseMatrix4X4f(m_DrawFrameContext.m_viewMatrix);
    } else {
        cout << "Default camera." << endl;
        Vector3f position = {0, 0, 5}, lookAt = {0, 0, 0}, up = {0, 1, 0};
        BuildViewMatrix(m_DrawFrameContext.m_viewMatrix, position, lookAt, up);
    }

    auto pCamera = scene.GetCamera(pCameraNode->GetSceneObjectRef());

    float fieldOfView = dynamic_pointer_cast<SceneObjectPerspectiveCamera>(pCamera)->GetFov();
    const GfxConfiguration &conf = g_pApp->GetConfiguration();

    float screenAspect = (float) conf.screenWidth / (float) conf.screenHeight;

    BuildPerspectiveFovRHMatrix(m_DrawFrameContext.m_projectionMatrix, fieldOfView, screenAspect,
                                pCamera->GetNearClipDistance(), pCamera->GetFarClipDistance());
}

void OpenGLGraphicsManager::CalculateLights() {
    auto &scene = g_pSceneManager->GetSceneForRendering();
    auto pLightNode = scene.GetFirstLightNode();
    if (pLightNode) {
        m_DrawFrameContext.m_lightPosition = {0.0f, 0.0f, 0.0f};
        TransformCoord(m_DrawFrameContext.m_lightPosition, *pLightNode->GetCalculatedTransform());

        auto pLight = scene.GetLight(pLightNode->GetSceneObjectRef());
        if (pLight) {
            m_DrawFrameContext.m_lightColor = pLight->GetColor().Value;
        }
    } else {
        m_DrawFrameContext.m_lightPosition = {10.0f, 10.0f, 10.0f};
        m_DrawFrameContext.m_lightColor = {1.0f, 1.0f, 1.0f, 1.0f};
    }
}
