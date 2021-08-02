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

//    GLenum a;
//
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(1.0f, 0.85f, 0.35f);
//    glBegin(GL_TRIANGLES);
//    {
//        glVertex3f(0.0, 0.6, 0.0);
//        glVertex3f(-0.2, -0.3, 0.0);
//        glVertex3f(0.2, -0.3, 0.0);
//    }
//    glEnd();
//    glFlush();

    Me::g_pGraphicsManager->Clear();
    Me::g_pGraphicsManager->Draw();
    glFlush();
    [_openGLContext flushBuffer];
}

- (void) mouseDown:(NSEvent*)event {
    [self drawRect:[self frame]];
}

- (instancetype)initWithFrame:(NSRect)frameRect {
    NSLog(@"initWithFrame 0");
    self = [super initWithFrame:frameRect];

    _openGLContext = [[NSOpenGLContext alloc] initWithFormat:_pixelFormat shareContext:nil];

    [_openGLContext makeCurrentContext];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_surfaceNeedsUpdate:)
                                                 name:NSViewGlobalFrameDidChangeNotification
                                               object:self];

    [_openGLContext setView:self];
    NSLog(@"initWithFrame 1");
    return self;
}

- (void)lockFocus {
    NSLog(@"lockFocus");
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
