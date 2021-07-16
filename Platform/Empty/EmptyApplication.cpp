#include "BaseApplication.hpp"
#include "MemoryManager.hpp"
#include "GraphicsManager.hpp"

namespace Me {
    GfxConfiguration config(8, 8, 8, 8, 24, 0, 0, 960, 540, "Empty Application");
    IApplication *g_pApp = static_cast<IApplication *>(new BaseApplication(config));
    GraphicsManager *g_pGraphicsManager = static_cast<GraphicsManager *>(new GraphicsManager);
    MemoryManager *g_pMemoryManager = static_cast<MemoryManager *>(new MemoryManager);
}