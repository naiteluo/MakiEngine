#include "gtest/gtest.h"
#include <iostream>
#include "GraphicsManager.hpp"
#include "BaseApplication.hpp"
#include "MemoryManager.hpp"
#include "AssetLoader.hpp"
#include "BMP.hpp"

using namespace Me;
using namespace std;

namespace Me {
    class TestApplication : public BaseApplication {
    public:
        using BaseApplication::BaseApplication;

        virtual int Initialize();

        virtual void Finalize();

    private:
        Image m_Image;
    };
}

namespace Me {
    GfxConfiguration config(8, 8, 8, 8);
    IApplication *g_pApp = static_cast<IApplication *>(new TestApplication(config));
    MemoryManager *g_pMemoryManager = static_cast<MemoryManager *>(new MemoryManager);
    GraphicsManager *g_pGraphicsManager = static_cast<GraphicsManager *>(new GraphicsManager);
}

const char * right_path = "Textures/icelogo-color.bmp";

int TestApplication::Initialize() {
    int result;
    result = BaseApplication::Initialize();
    BaseApplication::m_bQuit = true;
    return result;
}

void TestApplication::Finalize() {
    BaseApplication::Finalize();
}

TEST(TextureLoad, InitializeAndFinalize) {
    int ret;
    if ((ret = g_pApp->Initialize()) != 0) {
        printf("App Initialize failed. Exit now.");
    }
    EXPECT_EQ(ret, 0);
    g_pApp->Finalize();
}