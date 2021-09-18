#include <stdio.h>
#include <climits>
#include <cstring>
#include "CocoaOpenGLApplication.h"

#import "GLView.h"

using namespace Me;

int CocoaOpenGLApplication::Initialize() {
    int result = 0;

    result = CocoaApplication::Initialize();

    if (!result) {
        NSOpenGLPixelFormatAttribute attrs[] = {
                NSOpenGLPFAAccelerated,
                NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
                NSOpenGLPFAColorSize, 32,
                NSOpenGLPFADepthSize, 24,
                NSOpenGLPFAStencilSize, 8,
                NSOpenGLPFADoubleBuffer,
                NSOpenGLPFAAccelerated,
                NSOpenGLPFAMultisample,
                NSOpenGLPFASampleBuffers, 1,
                NSOpenGLPFASamples, 4, // 4x MSAA
                0
        };

        // create customized view
        GLView *pGLView = [GLView new];
        // create pixel format
        pGLView.pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if ([pGLView pixelFormat] == nil) {
            NSLog(@"No valid matching OpenGL Pixel Format found");
            [pGLView release];
            return -1;
        }
        // init view (create opengl context)
        [pGLView initWithFrame:CGRectMake(0, 0, m_Config.screenWidth, m_Config.screenHeight)];
        // add view to window
        [m_pWindow setContentView:pGLView];
    }

    return result;
}

void CocoaOpenGLApplication::Finalize() {
    CocoaApplication::Finalize();
}

void CocoaOpenGLApplication::Tick() {
    CocoaApplication::Tick();
    [[m_pWindow contentView] setNeedsDisplay:YES];
}

