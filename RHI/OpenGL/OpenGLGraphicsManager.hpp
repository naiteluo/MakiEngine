#pragma once

#include "GraphicsManager.hpp"

namespace Me {
    class OpenGLGraphicsManager : public GraphicsManager {
    public:
        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

        virtual void Clear();

        virtual void Draw();

    private:

    };
}