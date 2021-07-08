#include "BaseApplication.hpp"

int Me::BaseApplication::Initialize() {
    m_bQuit = false;
    return 0;
}

void Me::BaseApplication::Finalize() {

}

void Me::BaseApplication::Tick() {

}

bool Me::BaseApplication::IsQuit() {
    return m_bQuit;
}
