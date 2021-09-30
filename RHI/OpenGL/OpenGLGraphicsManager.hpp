#pragma once

#include "GraphicsManager.hpp"
#include "GeomMath.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "glad/glad.h"

namespace Me {
    class OpenGLGraphicsManager : public GraphicsManager {
    public:
        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

        virtual void Clear();

        virtual void Draw();

    private:

        bool SetPerBatchShaderParameters(const char *paramName, float *param);

        bool SetPerFrameShaderParameters();

        void InitializeBuffers();

        void RenderBuffers();

        void CalculateCamera();

        void CalculateLights();

        bool InitializeShader(const char *vsFilename, const char *fsFilename);

    private:
        unsigned int m_vertexShader;
        unsigned int m_fragmentShader;
        unsigned int m_shaderProgram;

        struct DrawFrameContext {
            Matrix4X4f m_worldMatrix;
            Matrix4X4f m_viewMatrix;
            Matrix4X4f m_projectionMatrix;
            Vector3f m_lightPosition;
            Vector4f m_lightColor;
        };

        struct DrawBatchContext {
            GLuint vao;
            GLenum mode;
            GLenum type;
            GLsizei count;
            std::shared_ptr<Matrix4X4f> transform;
        };

        DrawFrameContext m_DrawFrameContext;
        std::vector<DrawBatchContext> m_VAO;
        std::unordered_map<std::string, unsigned int> m_Buffers;
    };
}