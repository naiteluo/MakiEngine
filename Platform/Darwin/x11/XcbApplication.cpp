#include "XcbApplication.hpp"

int Me::XcbApplication::Initialize() {
    int result;

    uint32_t mask = 0;
    uint32_t values[3];

    // call base class initialization
    result = BaseApplication::Initialize();
    if (result != 0) {
        exit(result);
    }

    if (!m_pConn) {
        // establish connection to X server
        m_pConn = xcb_connect(0, 0);
    }

    if (!m_pScreen) {
        // get the first screen
        m_pScreen = xcb_setup_roots_iterator(xcb_get_setup(m_pConn)).data;
        m_nVi = m_pScreen->root_visual;
    }

    // get the root window
    m_Window = m_pScreen->root;

    // create xid's for colormap
    xcb_colormap_t colormap = xcb_generate_id(m_pConn);
    xcb_create_colormap(m_pConn, XCB_COLORMAP_ALLOC_NONE, colormap, m_Window, m_nVi);

    // create window
    m_Window = xcb_generate_id(m_pConn);
    mask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
    values[0] = XCB_GC_GRAPHICS_EXPOSURES | XCB_EVENT_MASK_KEY_PRESS;
    values[1] = colormap;
    values[2] = 0;
    xcb_create_window(m_pConn,                 /* connection */
                      XCB_COPY_FROM_PARENT,    /* depth */
                      m_Window,                /* window ID */
                      m_pScreen->root,         /* parent window */
                      20, 20,                  /* x, y */
                      m_Config.screenWidth,    /* width */
                      m_Config.screenHeight,   /* height */
                      10,                      /* boarder width */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class */
                      m_nVi,  /* visual */
                      mask, values);           /* masks */

    /* set the title of the window */
    xcb_change_property(m_pConn, XCB_PROP_MODE_REPLACE, m_Window,
                        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                        strlen(m_Config.appName), m_Config.appName);

    // map the window on the screen
    xcb_map_window(m_pConn, m_Window);

    xcb_flush(m_pConn);

    return result;
}

void Me::XcbApplication::Finalize() {
    xcb_disconnect(m_pConn);
}

void Me::XcbApplication::Tick() {
    xcb_generic_event_t *pEvent;
    pEvent = xcb_poll_for_event(m_pConn);
    if (pEvent) {
        // handle x event
        switch (pEvent->response_type & ~0x80) {
            // XCB_EXPOSE may not be fired
            case XCB_VISIBILITY_NOTIFY:
            case XCB_EXPOSE:
                break;
            case XCB_KEY_PRESS:
                BaseApplication::m_bQuit = true;
                break;
        }
        free(pEvent);
    } else {
        if (xcb_connection_has_error(m_pConn)) {
            m_bQuit = true;
        } else {
            OnDraw();
        }
    }
}
