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

    std::string scene_file_name = "Scene/test1.ogex";
    std::string preprocess_flag = "-yes";

    if (argc > 1) {
        scene_file_name = argv[1];
    }

    if (argc > 2) {
        preprocess_flag = argv[2];
    }

    g_pApp->GetConfiguration().preprocessCameraMatrix = strcmp(preprocess_flag.c_str(), "-no");

    if ((ret = g_pSceneManager->LoadScene(scene_file_name.c_str())) != 0) {
        printf("Unable to load scene: %s\n", scene_file_name.c_str());
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
