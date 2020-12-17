//
// Created by romaonishuk on 27.10.19.
//

#include "x_window_impl.hpp"

#include "font.hpp"
#include "logger.hpp"

namespace Gui {
XWindowImpl::XWindowImpl(const GlyphParams& params)
{
    XInitThreads();
    m_display = XOpenDisplay(nullptr);
    if(!m_display) {
        throw std::runtime_error("XWindowImpl display initialisation failed!");
    }

    CreateWindow(params);
    CreateGraphicContext();

    std::cout << "XWindowImpl created!" << std::endl;
}

XWindowImpl::XWindowImpl(const GlyphParams& params, XWindowImpl* parentImpl):
    m_display(parentImpl->m_display),
    m_gc(parentImpl->m_gc)
{
    m_isChild = true;
    m_window = XCreateSimpleWindow(
        m_display, parentImpl->m_window, params.x, params.y, params.width, params.height, 0, 1, 0xC0C0C0);

    // TODO(rmn): investigate event propagation
    XSelectInput(m_display, m_window,
        ClientMessage | ExposureMask | ButtonPress | ButtonRelease | MotionNotify | FocusIn | FocusOut | Expose |
            GraphicsExpose | CreateNotify | DestroyNotify | SubstructureNotifyMask | PointerMotionMask);
}

XWindowImpl::~XWindowImpl()
{
    if(!m_isChild) {
        XDestroyWindow(m_display, m_window);
        XFreeGC(m_display, m_gc);
        XCloseDisplay(m_display);

        Logger::Get().Log("Window closed!");
    }
}

void XWindowImpl::CreateWindow(const GlyphParams& params)
{
    auto screen = DefaultScreen(m_display);
    auto visual = DefaultVisual(m_display, screen);
    auto depth = DefaultDepth(m_display, screen);
    XSetWindowAttributes attributes;
    attributes.background_pixel = Color::kGray;  // XWhitePixel(m_display, screen);

    // TODO(rmn): check errors
    m_window = XCreateWindow(m_display, XRootWindow(m_display, screen), params.x, params.y, params.width, params.height,
        0, depth, InputOutput, visual, CWBackPixel, &attributes);

    Atom wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &wmDeleteMessage, 1);

    // TODO(rmn): this should be moved to event manager
    XSelectInput(m_display, m_window,
        ClientMessage | ExposureMask | ButtonPress | ButtonRelease | MotionNotify | FocusIn | FocusOut | Expose |
            GraphicsExpose | CreateNotify | DestroyNotify | SubstructureNotifyMask | PointerMotionMask);

    /* make the window actually appear on the screen. */
    XMapWindow(m_display, m_window);

    /* flush all pending requests to the X server. */
    XFlush(m_display);
}

void XWindowImpl::CreateGraphicContext()
{
    unsigned int line_width = 0; /* line width for the GC.       */
    int line_style = LineSolid;  /* style for lines drawing and  */
    int cap_style = CapRound;    /* style of the line's edje and */
    int join_style = JoinRound;  /*  joined lines.		*/

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
}

void XWindowImpl::DrawText(const Point& text_position, std::string text)
{
    XTextItem item;
    item.chars = const_cast<char*>(text.c_str());
    item.nchars = text.size();
    item.delta = 0;
    item.font = None;

    XDrawText(m_display, m_window, m_gc, text_position.x, text_position.y, &item, 1);
}

void XWindowImpl::DrawText(const GlyphParams& params, const std::string& text, Alignment alignment)
{
    auto* fontInfo = XQueryFont(m_display, Lexi::FontManager::Get().GetFontId());
    if(!fontInfo) {
        std::cout << "Failed to load font information" << std::endl;
        return;
    }

    int direction_return = 0;
    int font_ascent_return = 0, font_descent_return = 0;
    XCharStruct overall_return;

    XTextExtents(fontInfo, text.c_str(), text.size(), &direction_return, &font_ascent_return, &font_descent_return,
        &overall_return);

    Point point;
    // TODO(rmn): errors + too long/big words + other cases where descent !=0 + include bearing
    switch(alignment) {
        case Alignment::kCenter:
            point.x = params.x + params.width / 2 - overall_return.width / 2;
            point.y = params.y + params.height / 2 + overall_return.ascent / 2;
            break;
        case Alignment::kRight:
            point.x = params.x + params.width - overall_return.width;
            point.y = params.y + params.height / 2 + overall_return.ascent / 2;
            break;
        case Alignment::kLeft:
            point.x = params.x;
            point.y = params.y + params.height / 2 + overall_return.ascent / 2;
            break;
    }

    DrawText(point, text);
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
    SetForeground(color);  //  TODO(rmn): this should be fixed
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

void XWindowImpl::ClearGlyph(const GlyphParams& p, bool sendExposureEvent = false)
{
    XClearArea(m_display, m_window, p.x, p.y, p.width, p.height, sendExposureEvent);
}

void XWindowImpl::Resize(width_t width, height_t height)
{
    XWindowAttributes attr;
    XGetWindowAttributes(m_display, m_window, &attr);
    if(attr.height != height || attr.width != width) {
        XResizeWindow(m_display, m_window, width, height);
    }
}

void XWindowImpl::SetFontPath(const std::string& path)
{
    int fontsCount = 0;
    char** currentPath = XGetFontPath(m_display, &fontsCount);

    std::vector<char*> v;
    v.reserve(fontsCount + 1);
    for(int i = 0; i < fontsCount; ++i) {
        v.push_back(currentPath[i]);
    }
    if(!path.empty()) {
        v.push_back(const_cast<char*>(path.c_str()));
    }

    XSetFontPath(m_display, v.data(), v.size());
    XFreeFontPath(currentPath);
}

bool XWindowImpl::ChangeFont(Lexi::Font& font)
{
    auto screen = DefaultScreen(m_display);
    int res_x = DisplayWidth(m_display, screen) / (DisplayWidthMM(m_display, screen) / 25.4);
    int res_y = DisplayHeight(m_display, screen) / (DisplayHeightMM(m_display, screen) / 25.4);

    auto getWight = [&] {
        if(font.m_weight == Lexi::Font::Weight::kBold) {
            return "-bold";
        }
        return "-medium";
    };

    auto getSlant = [&] {
        if(font.m_slant == Lexi::Font::Slant::kItalic) {
            return "-i";
        }
        return "-r";
    };

    std::string fontNamePattern = "-*-";
    fontNamePattern += font.m_name;
    fontNamePattern += getWight();
    fontNamePattern += getSlant();
    fontNamePattern += "-*-*-";
    fontNamePattern +=
        std::to_string(font.m_fontSize) + "-*-" + std::to_string(res_x) + "-" + std::to_string(res_y) + "-*-*-*-*";

    auto* result = XLoadQueryFont(m_display, fontNamePattern.c_str());
    if(result) {
        XSetFont(m_display, m_gc, result->fid);
        font.m_fontId = result->fid;

        font.m_charWidth = result->max_bounds.width;
        font.m_charAscent = result->max_bounds.ascent;
        font.m_charDescent = result->max_bounds.descent;
        font.m_charHeight = result->max_bounds.ascent + result->max_bounds.descent;
        //        XFreeFont(m_display, fontInfo);

        return true;
    }

    std::cout << "Failed to set font to " << font.m_name << std::endl;
    return false;
}

std::set<Lexi::FontName> XWindowImpl::GetFontList()
{
    int maxNames = 1000;
    int countReturn = 0;
    XFontStruct* info_return;

    char** fontsInfoList = XListFontsWithInfo(m_display, "*", maxNames, &countReturn, &info_return);
    if(!fontsInfoList) {
        std::cout << "Failed to retrieve XListFontsWithInfo" << std::endl;
        return {};
    }

    std::cout << "Received " << countReturn << " fonts" << std::endl;

    std::set<Lexi::FontName> fontsList;

    for(int i = 0; i < countReturn; ++i) {
        auto fontName = Lexi::ParseXLFDName(fontsInfoList[i]);
        if(!fontName.IsEmpty()) {
            fontsList.emplace(std::move(fontName));
        }
    }

    XFreeFontInfo(fontsInfoList, info_return, countReturn);

    return fontsList;
}

}  // namespace Gui
