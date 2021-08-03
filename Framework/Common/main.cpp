#include <cstdio>
#include <chrono>
#include <thread>
#include "IApplication.hpp"
#include "MemoryManager.hpp"
#include "GraphicsManager.hpp"

using namespace Me;

namespace Me {
    extern IApplication *g_pApp;
    extern MemoryManager *g_pMemoryManager;
    extern GraphicsManager *g_pGraphicsManager;
}

int main(int argc, char **argv) {
    int ret;

    if ((ret = g_pApp->Initialize()) != 0) {
        printf("App Initialize failed. Exit now.");
        return ret;
    }

    if ((ret = g_pMemoryManager->Initialize()) != 0) {
        printf("Memory Manager Initialize failed. Exit now.");
        return ret;
    }

    if ((ret = g_pGraphicsManager->Initialize()) != 0) {
        printf("Graphics Manager Initialize failed. Exit now.");
        return ret;
    }

    printf("Loop Start...");

    while (!g_pApp->IsQuit()) {
        g_pApp->Tick();
        g_pMemoryManager->Tick();
        g_pGraphicsManager->Tick();
        // TODO what for?
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }

    g_pGraphicsManager->Finalize();
    g_pMemoryManager->Finalize();
    g_pApp->Finalize();

    return 0;
}
