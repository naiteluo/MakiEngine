#include "gtest/gtest.h"
#include "AssetLoader.hpp"
#include "MemoryManager.hpp"
#include "OGEX.hpp"

namespace Me {
    MemoryManager *g_pMemoryManager = new MemoryManager();
}

TEST(OgexParser, Load) {
    Me::g_pMemoryManager->Initialize();

    Me::AssetLoader asset_loader;
    string ogex_text = asset_loader.SyncOpenAndReadTextFileToString("Scene/Example.ogex");

    Me::OgexParser *ogex_parser = new Me::OgexParser();
    std::unique_ptr<Me::Scene> p_scene = ogex_parser->Parse(ogex_text);
    delete ogex_parser;

    std::cout << *p_scene->SceneGraph << std::endl;
    Me::BaseSceneNode& node = *p_scene->SceneGraph;

    EXPECT_EQ((*p_scene->SceneGraph).GetStrName(), "OGEX Scene");

    Me::g_pMemoryManager->Finalize();
    delete Me::g_pMemoryManager;
}
