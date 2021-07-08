#include <stdio.h>
#include "IApplication.hpp"

using namespace Me;

namespace Me {
    extern IApplication *g_pApp;
}

int main(int argc, char **argv) {
    int ret;

    if ((ret = g_pApp->Initialize()) != 0) {
        printf("App Initialize failed. Exit now.");
        return ret;
    }

    printf("Loop Start...");

    while (!g_pApp->IsQuit()) {
        g_pApp->Tick();
    }

    g_pApp->Finalize();

    return 0;
}
