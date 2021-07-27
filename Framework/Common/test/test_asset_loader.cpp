#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "AssetLoader.hpp"
#include "MemoryManager.hpp"
#include "BMP.hpp"

using namespace Me;

namespace Me {
    MemoryManager *g_pMemoryManager = new MemoryManager();
}

TEST(AssetLoader, BasicText) {
    g_pMemoryManager->Initialize();
    AssetLoader assetLoader;
    std::string str = assetLoader.SyncOpenAndReadTextFileToString("data.txt");
    std::cout << str;
    EXPECT_STREQ(str.c_str(), "hello data\nhello game");
}

TEST(AssetLoader, CorrectBmpImage) {
    g_pMemoryManager->Initialize();
    AssetLoader assetLoader;
    BmpParser parser;
    Buffer buf = assetLoader.SyncOpenAndReadBinary("Textures/icelogo-color.bmp");
    Image img = parser.Parse(buf);
    std::cout << img;
    EXPECT_EQ(img.Width, 512);
    EXPECT_EQ(img.Height, 512);
}

TEST(AssetLoader, WrongImagePath) {
    g_pMemoryManager->Initialize();
    int b = 2;
    int * const a = &b;
    ASSERT_DEATH({
                     AssetLoader assetLoader;
                     BmpParser parser;
                     Buffer buf = assetLoader.SyncOpenAndReadBinary("Textures/not-exist.bmp");
                     Image img = parser.Parse(buf);
                 }, "");
}
