#include <cstdio>
#include <chrono>
#include <thread>
#include "BaseApplication.hpp"

using namespace Me;

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

    if ((ret = g_pAssetLoader->Initialize()) != 0) {
        printf("Asset Loader Initialize failed. Exit now.");
        return ret;
    }

    if ((ret = g_pSceneManager->Initialize()) != 0) {
        printf("Scene Manager Initialize failed. Exit now.");
        return ret;
    }

//    g_pSceneManager->LoadScene("Scene/paimon-and-objects.ogex");
//    g_pSceneManager->LoadScene("Scene/test1.ogex");
    g_pSceneManager->LoadScene("Scene/hutao.ogex");
//    g_pSceneManager->LoadScene("Scene/aili.ogex");
    g_pApp->GetConfiguration().preprocessCameraMatrix = true;

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
