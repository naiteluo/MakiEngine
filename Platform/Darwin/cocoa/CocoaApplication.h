#include "BaseApplication.hpp"

#ifdef __OBJC__
#include <Cocoa/Cocoa.h>
#endif

namespace Me {
    class CocoaApplication : public BaseApplication {
    public:
        CocoaApplication(GfxConfiguration &config) : BaseApplication(config) {};

        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

    protected:
#ifdef __OBJC__
        NSWindow* m_pWindow;
#endif
    };
}