//
// Created by romaonishuk on 27.10.19.
//

#include "x_window_impl.hpp"
#include "logger.hpp"

namespace Gui {
XWindowImpl::XWindowImpl(const GlyphParams& params)
{
    m_display = XOpenDisplay(nullptr);
    if(!m_display) {
        throw std::runtime_error("XWindowImpl display initialisation failed!");
    }

    CreateWindow(params.x, params.y, params.width, params.height);
    CreateGraphicContext();

    Logger::Get().Log("XWindowImpl created!");
}

// TODO(rmn): refactor this. Testing menu items
XWindowImpl::XWindowImpl(const GlyphParams params, XWindowImpl* parentImpl):
    m_display(parentImpl->m_display),
    m_gc(parentImpl->m_gc)
{
    m_is_child = true;
    m_window = XCreateSimpleWindow(
        m_display, parentImpl->m_window, params.x, params.y, params.width, params.height, 0, 1, 0xC0C0C0);

    // TODO(rmn): investigate event propagation
    XSelectInput(m_display, m_window,
        ClientMessage | ExposureMask | ButtonPress | ButtonRelease | MotionNotify | FocusIn | FocusOut | Expose |
            GraphicsExpose | CreateNotify | DestroyNotify | SubstructureNotifyMask | PointerMotionMask);
    //
    //    XFlush(m_display);
}

XWindowImpl::~XWindowImpl()
{
    if(!m_is_child) {
        XDestroyWindow(m_display, m_window);
        XFreeGC(m_display, m_gc);
        XCloseDisplay(m_display);

        Logger::Get().Log("Window closed!");
    }
}

void XWindowImpl::CreateWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    auto screen = DefaultScreen(m_display);
    auto visual = DefaultVisual(m_display, screen);
    auto depth = DefaultDepth(m_display, screen);
    XSetWindowAttributes attributes;
    attributes.background_pixel = 0xC0C0C0;  // XWhitePixel(m_display, screen);

    // TODO(rmn): check errors
    m_window = XCreateWindow(m_display, XRootWindow(m_display, screen), x, y, width, height, 5, depth, InputOutput,
        visual, CWBackPixel, &attributes);

    Atom wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &wmDeleteMessage, 1);

    // TODO(rmn): this should be moved to event manager
    XSelectInput(m_display, m_window,
        ClientMessage | ExposureMask | ButtonPress | ButtonRelease | MotionNotify | FocusIn | FocusOut | Expose |
            GraphicsExpose | CreateNotify | DestroyNotify | SubstructureNotifyMask);

    /* make the window actually appear on the screen. */
    XMapWindow(m_display, m_window);

    /* flush all pending requests to the X server. */
    XFlush(m_display);
}

void XWindowImpl::CreateGraphicContext()
{
    unsigned int line_width = 2; /* line width for the GC.       */
    int line_style = LineSolid;  /* style for lines drawing and  */
    int cap_style = CapButt;     /* style of the line's edje and */
    int join_style = JoinBevel;  /*  joined lines.		*/

    m_gc = XCreateGC(m_display, m_window, 0, nullptr);
    if(m_gc == nullptr) {
        throw std::runtime_error("XCreateGC failed!");
    }

    /* define the style of lines that will be drawn using this GC. */
    XSetLineAttributes(m_display, m_gc, line_width, line_style, cap_style, join_style);

    /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(m_display, m_gc, FillSolid);
}

void XWindowImpl::DrawRectangle(const Point& point, const width_t width, const height_t height)
{
    XDrawRectangle(m_display, m_window, m_gc, point.x, point.y, width, height);
    // XFlush (m_display);
}

void XWindowImpl::DrawText(const Point& text_position, std::string text)
{
    XDrawString(m_display, m_window, m_gc, text_position.x, text_position.y, text.c_str(), text.length());
}

void XWindowImpl::DrawLine(const Point& start_point, const Point& end_point)
{
    XDrawLine(m_display, m_window, m_gc, start_point.x, start_point.y, end_point.x, end_point.y);
}

void XWindowImpl::SetForeground(const int color)
{
    XSetForeground(m_display, m_gc, color);
}

void XWindowImpl::FillRectangle(const Point& point, const width_t width, const height_t height, const Color color)
{
    SetForeground(color);
    XFillRectangle(m_display, m_window, m_gc, point.x, point.y, width, height);
}

void XWindowImpl::ShowWindow()
{
    XMapWindow(m_display, m_window);
}

void XWindowImpl::HideWindow()
{
    XUnmapWindow(m_display, m_window);
}

void XWindowImpl::ClearWindow()
{
    XClearWindow(m_display, m_window);
}

}  // namespace Gui
