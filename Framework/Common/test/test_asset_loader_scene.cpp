#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "AssetLoader.hpp"
#include "MemoryManager.hpp"
#include "SceneManager.hpp"

using namespace Me;
using namespace std;

template<typename T>
static ostream &operator<<(ostream &out, unordered_map<string, shared_ptr<T>> map) {
    for (auto p : map) {
        out << *p.second << endl;
    }
}

namespace Me {
    MemoryManager *g_pMemoryManager = new MemoryManager();
    SceneManager *g_pSceneManager = new SceneManager();
    AssetLoader *g_pAssetLoader = new AssetLoader();
}

TEST(Scene, Load) {

    bool success = false;

    g_pMemoryManager->Initialize();
    g_pSceneManager->Initialize();
    g_pAssetLoader->Initialize();

    g_pSceneManager->LoadScene("Scene/cube.ogex");
    auto &scene = g_pSceneManager->GetSceneForRendering();

    cout << "Dump of Cameras" << endl;
    cout << "-----------------" << endl;

    auto pCameraNode = scene.GetFirstCameraNode();
    while (pCameraNode) {
        auto pNode = scene.GetCamera(pCameraNode->GetSceneObjectRef());
        success = true;
        cout << *pNode << endl;
        pCameraNode = scene.GetNextCameraNode();
    }

    g_pAssetLoader->Finalize();
    g_pSceneManager->Finalize();
    g_pMemoryManager->Initialize();

    EXPECT_TRUE(success);
}