#include <stdio.h>
#include <climits>
#include <cstring>
#include "CocoaOpenGLApplication.h"

#import "GLView.h"

using namespace Me;

int CocoaOpenGLApplication::Initialize() {
    NSLog(@"CocoaOpenGLApplication::Initialize");
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

        NSLog(@"Before GLView *pGLView = [GLView new];");
        GLView *pGLView = [GLView new];
        NSLog(@"After GLView *pGLView = [GLView new];");
        pGLView.pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        NSLog(@"CocoaOpenGLApplication::Initialize");
        if ([pGLView pixelFormat] == nil) {
            NSLog(@"No valid matching OpenGL Pixel Format found");
            [pGLView release];
            return -1;
        }

        NSLog(@"pGLView initWithFrame:CGRectMake");
        [pGLView initWithFrame:CGRectMake(0, 0, m_Config.screenWidth, m_Config.screenHeight)];

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

