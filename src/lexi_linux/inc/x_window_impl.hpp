//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_X_WINDOW_IMPL_CPP_HPP
#define LEXI_X_WINDOW_IMPL_CPP_HPP

#include "window_impl.hpp"

#include <X11/Xlib.h>

namespace Gui
{
class XWindowImpl : public WindowImpl
{
public:
    XWindowImpl(const GlyphParams);
    ~XWindowImpl();

    void DrawRectangle(const Point& point, const width_t width, const height_t height) override;
    void DrawText(const Point& text_position, std::string text) override;
    void DrawLine(const Point& start_point, const Point& end_point) override;

    void SetForeground(const int color) override;

    void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color);

    // TODO(rmn): undo public!!!!
public:
    void CreateGraphicContext();

    void CreateWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    ::Display *m_display;
    ::Window m_window;
    ::GC m_gc;
};
}

#endif //LEXI_X_WINDOW_IMPL_CPP_HPP
