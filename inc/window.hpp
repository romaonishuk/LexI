//
// Created by romaonishuk on 27.10.19.
//

#ifndef LEXI_WINDOW_HPP
#define LEXI_WINDOW_HPP

#include <memory>
#include "glyphs.hpp"
// TODO: WindowImpl should be private for Window
#include "window_impl.hpp"
#include "types.hpp"

namespace Gui
{
    class Window : public ICompositeGlyph
    {
    public:
        Window();
        ~Window();

        int GetEvent();

        void Draw(Gui::Window *) override;
        void DrawRectangle(const Point& point, const width_t width, const height_t height) const;
        void DrawText(const Point& text_position, std::string text) const;
        void DrawLine(const Point& start_point, const Point& end_point) const;
        
        void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color);
        
        void SetForeground(int color) const;
        void ProcessEvent(int event_type);
    private:
//        class WindowImpl;
        std::unique_ptr<WindowImpl> m_window_impl;
    };
}

#endif // LEXI_WINDOW_HPP