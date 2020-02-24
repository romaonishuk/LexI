//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_WINDOW_IMPL_HPP
#define LEXI_WINDOW_IMPL_HPP

#include <string>

#include "types.hpp"

namespace Gui
{
class WindowImpl
{
public:
    WindowImpl() = default;
    virtual ~WindowImpl() = default;

    virtual void DrawRectangle(const Point& point, const width_t width, const height_t height) = 0;
    virtual void DrawText(const Point& text_position, std::string text) = 0;
    virtual void DrawLine(const Point& start_point, const Point& end_point) = 0;
    virtual void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color)= 0;

    virtual void SetForeground(const int color) = 0;
};
}
#endif //LEXI_WINDOW_IMPL_HPP
