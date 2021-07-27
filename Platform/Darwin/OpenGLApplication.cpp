#include <stdio.h>
#include <climits>
#include <cstring>
#include <X11/Xlib-xcb.h>
#include "OpenGLApplication.hpp"
#include "OpenGL/OpenGLGraphicsManager.hpp"
#include "MemoryManager.hpp"

namespace Me {
    GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 960, 540, "Game Engine From Scratch (Darwin)");
    IApplication *g_pApp = static_cast<IApplication *>(new OpenGLApplication(config));
    GraphicsManager *g_pGraphicsManager = static_cast<GraphicsManager *>(new OpenGLGraphicsManager);
    MemoryManager *g_pMemoryManager = static_cast<MemoryManager *>(new MemoryManager);
}

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension) {
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string. Don't be fooled by sub-strings,
       etc. */
    for (start = extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;

        start = terminator;
    }

    return false;
}

static bool ctxErrorOccurred = false;

static int ctxErrorHandler(Display *dpy, XErrorEvent *ev) {
    ctxErrorOccurred = true;
    return 0;
}

int Me::OpenGLApplication::Initialize() {
    int result;

    int default_screen;
    GLXFBConfig *fb_configs;
    GLXFBConfig fb_config;
    int num_fb_configs = 0;

    XVisualInfo *vi;
    GLXWindow glxWindow;
    const char *glxExts;

    // Get a matching FB config
    // 这是我们期望获得的FrameBuffer配置
    static int visual_attribs[] =
            {
                    GLX_X_RENDERABLE, True,
                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                    GLX_RENDER_TYPE, GLX_RGBA_BIT,
                    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
                    GLX_RED_SIZE, static_cast<int>(INT_MAX & m_Config.redBits),
                    GLX_GREEN_SIZE, static_cast<int>(INT_MAX & m_Config.greenBits),
                    GLX_BLUE_SIZE, static_cast<int>(INT_MAX & m_Config.blueBits),
                    GLX_ALPHA_SIZE, static_cast<int>(INT_MAX & m_Config.alphaBits),
                    GLX_DEPTH_SIZE, static_cast<int>(INT_MAX & m_Config.depthBits),
                    GLX_STENCIL_SIZE, static_cast<int>(INT_MAX & m_Config.stencilBits),
                    GLX_DOUBLEBUFFER, True,
                    //GLX_SAMPLE_BUFFERS  , 1,
                    //GLX_SAMPLES         , 4,
                    None
            };

    // open Xlib display
    m_pDisplay = XOpenDisplay(NULL);
    if (!m_pDisplay) {
        fprintf(stderr, "Can't open display\n");
        return -1;
    }

    default_screen = DefaultScreen(m_pDisplay);

    gladLoadGLX(m_pDisplay, default_screen);

    // Query framebuffer configurations
    // 罗列出缺省显示器所支持的符合上述条件的所有FrameBuffer格式，然后选择一个最好的（采样数最多的）
    fb_configs = glXChooseFBConfig(m_pDisplay, default_screen, visual_attribs, &num_fb_configs);
    if (!fb_configs || num_fb_configs == 0) {
        fprintf(stderr, "glXChooseFBConfig fail\n");
        return -1;
    }

    // TODO: 这块还是有些不明白 fb_config 和 visual的概念和用途
    // pick the fb config/visual with the most samples per pixel
    {
        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

        for (int i = 0; i < num_fb_configs; ++i) {
            XVisualInfo *vi = glXGetVisualFromFBConfig(m_pDisplay, fb_configs[i]);
            if (vi) {
                int samp_buf, samples;
                glXGetFBConfigAttrib(m_pDisplay, fb_configs[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(m_pDisplay, fb_configs[i], GLX_SAMPLES, &samples);

                printf("  Matching fbconfig %d, visual ID 0x%lx: SAMPLE_BUFFERS = %d,"
                       " SAMPLES = %d\n",
                       i, vi->visualid, samp_buf, samples);

                if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
                    best_fbc = i, best_num_samp = samples;
                if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                    worst_fbc = i, worst_num_samp = samples;
            }
            XFree(vi);
        }
        fb_config = fb_configs[best_fbc];
    }

    // get a visual
    vi = glXGetVisualFromFBConfig(m_pDisplay, fb_config);
    printf("Chosen visual ID = 0x%lx\n", vi->visualid);


    // establish connection to X server
    m_pConn = XGetXCBConnection(m_pDisplay);
    if (!m_pConn) {
        XCloseDisplay(m_pDisplay);
        fprintf(stderr, "Can't get xcb connection from display\n");
        return -1;
    }

    // acquire event queue ownership
    XSetEventQueueOwner(m_pDisplay, XCBOwnsEventQueue);

    // find xcb screen
    // 使用XCB来创建窗口并管理窗口，所以接下来做了一个同步，让XCB和Xlib都指向同一块屏幕（FrameBuffer）
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(m_pConn));
    for (int screen_num = vi->screen; screen_iter.rem && screen_num > 0; --screen_num, xcb_screen_next(&screen_iter)) {
        fprintf(stdout, "Screen index: %i \n", screen_iter.index);
    }
    m_pScreen = screen_iter.data;
    m_nVi = vi->visualid;

    result = XcbApplication::Initialize();
    if (result) {
        printf("Xcb Application initialize failed!");
        return -1;
    }

    // get the default screen's GLX extension list
    glxExts = glXQueryExtensionsString(m_pDisplay, default_screen);

    // create opengl context
    ctxErrorOccurred = false;
    int (*oldHandler)(Display *, XErrorEvent *) = XSetErrorHandler(&ctxErrorHandler);

    // 在OpenGL 3.0之前（不含）的版本与之后的版本的创建方法是不一样的
    // 版本越低，能够使用的OpenGL功能就越少
    // 进行了一些版本的探查，并根据探查结果选择最好的创建方式

    if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
        printf("glXCreateContextAttribsARB() not found ... using old-style GLX context\n");
        m_Context = glXCreateNewContext(m_pDisplay, fb_config, GLX_RGBA_TYPE, 0, True);
        if (!m_Context) {
            fprintf(stderr, "glxCreateNewContext failed\n");
            return -1;
        }
    } else {
        int context_attribs[] = {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                None
        };
        printf("Creating context\n");
        m_Context = glXCreateContextAttribsARB(m_pDisplay, fb_config, 0, True, context_attribs);
        XSync(m_pDisplay, False);
        if (!ctxErrorOccurred && m_Context) {
            printf("Created GL 3.0 context\n");
        } else {
            // GLX_CONTEXT_MAJOR_VERSION_ARB
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB
            context_attribs[3] = 0;

            ctxErrorOccurred = false;
            printf("Failed to create GL 3.0 context"
                   "... using old-styl GLX context\n");
            m_Context = glXCreateContextAttribsARB(m_pDisplay, fb_config, 0, True, context_attribs);
        }
    }

    // TODO: what for?
    XSync(m_pDisplay, False);

    XSetErrorHandler(oldHandler);

    if (ctxErrorOccurred || !m_Context) {
        fprintf(stderr, "Failed to create an OpenGL context\n");
        return -1;
    }

    // Verifying that context if a direct context
    if (!glXIsDirect(m_pDisplay, m_Context)) {
        printf("Indirect GLX rendering context obtained\n");
    } else {
        printf("Direct GLX rendering context obtained\n");
    }

    // create GLX window
    // 为了让GLX能够使用我们通过XCB创建出来的窗口，我们对窗口进行了一次转换，让它也绑定到GLX的对象当中
    glxWindow = glXCreateWindow(m_pDisplay, fb_config, m_Window, 0);
    if (!m_Window) {
        xcb_destroy_window(m_pConn, m_Window);
        glXDestroyContext(m_pDisplay, m_Context);
        fprintf(stderr, "glXCreateWindow failed\n");
        return -1;
    }

    m_Drawable = glxWindow;

    // make OpenGL context current
    if (!glXMakeContextCurrent(m_pDisplay, m_Drawable, m_Drawable, m_Context)) {
        xcb_destroy_window(m_pConn, m_Window);
        glXDestroyContext(m_pDisplay, m_Context);
        fprintf(stderr, "glXMakeContextCurrent failed\n");
        return -1;
    }

    XFree(vi);

    return result;
}

void Me::OpenGLApplication::Finalize() {
    XcbApplication::Finalize();
}

void Me::OpenGLApplication::Tick() {
    XcbApplication::Tick();
}

void Me::OpenGLApplication::OnDraw() {
    glXSwapBuffers(m_pDisplay, m_Drawable);

}
