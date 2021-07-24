#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "AssetLoader.hpp"
#include "MemoryManager.hpp"

using namespace Me;

namespace Me {
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

TEST(AssetLoader, Basic) {
    g_pMemoryManager->Initialize();

    AssetLoader assetLoader;
    std::string str = assetLoader.SyncOpenAndReadTextFileToString("data.txt");

    std::cout << str;

    g_pMemoryManager->Finalize();

    delete g_pMemoryManager;
}