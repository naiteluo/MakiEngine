#import "GLView.h"
#import <OpenGL/gl3.h>

#import "GraphicsManager.hpp"
namespace Me {
    extern GraphicsManager *g_pGraphicsManager;
}

@implementation GLView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    [_openGLContext makeCurrentContext];

    Me::g_pGraphicsManager->Clear();
    Me::g_pGraphicsManager->Draw();

    [_openGLContext flushBuffer];
}

- (void) mouseDown:(NSEvent*)event {
    [self drawRect:[self frame]];
}

- (instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];

    // create opengl context
    _openGLContext = [[NSOpenGLContext alloc] initWithFormat:_pixelFormat shareContext:nil];

    [_openGLContext makeCurrentContext];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_surfaceNeedsUpdate:)
                                                 name:NSViewGlobalFrameDidChangeNotification
                                               object:self];

    // fix `lockFoucs` not fired
    [_openGLContext setView:self];
    return self;
}

/**
 * not fired
 */
- (void)lockFocus {
    [super lockFocus];
    if ([_openGLContext view] != self) {
        [_openGLContext setView:self];
    }
    [_openGLContext makeCurrentContext];
}

- (void)update {
    [_openGLContext update];
}

- (void)_surfaceNeedsUpdate:(NSNotification *)notification {
    [self update];
}

- (void)dealloc {
    [_pixelFormat release];
    [_openGLContext release];

    [super dealloc];
}

@end

