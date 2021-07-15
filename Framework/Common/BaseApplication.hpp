#pragma once

#include "IApplication.hpp"
#include "GfxConfiguration.h"

namespace Me {
    class BaseApplication : implements IApplication {
    public:
        BaseApplication(GfxConfiguration &cfg);

        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

        virtual bool IsQuit();

        inline GfxConfiguration &GetConfiguration() {
            return m_Config;
        };

    protected:
        virtual void OnDraw() {};

    protected:
        // flag if need quit the main loop of the application
        static bool m_bQuit;
        GfxConfiguration m_Config;
    private:
        // hide the default construct to enforce a configuration
        BaseApplication() {};
    };
}