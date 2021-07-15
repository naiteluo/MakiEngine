#include "BaseApplication.hpp"
#include <iostream>

using namespace Me;

bool Me::BaseApplication::m_bQuit = false;

BaseApplication::BaseApplication(GfxConfiguration &cfg) : m_Config(cfg) {

}

int Me::BaseApplication::Initialize() {
    m_bQuit = false;
    std::wcout << m_Config;
    return 0;
}

void Me::BaseApplication::Finalize() {

}

void Me::BaseApplication::Tick() {

}

bool Me::BaseApplication::IsQuit() {
    return m_bQuit;
}
