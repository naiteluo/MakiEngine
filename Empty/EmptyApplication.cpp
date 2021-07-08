#include "BaseApplication.hpp"

namespace Me {
    BaseApplication g_App;
    IApplication *g_pApp = &g_App;
}