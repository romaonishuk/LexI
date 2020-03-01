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

#include "event_manager.hpp"

namespace Gui
{
    class Window : public ICompositeGlyph
    {
    public:
        Window(const GlyphParams);
        ~Window();

        void Draw(Gui::Window *) override;
        void DrawRectangle(const Point& point, const width_t width, const height_t height) const;
        void DrawText(const Point& text_position, std::string text) const;
        void DrawLine(const Point& start_point, const Point& end_point) const;
        
        void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color);
        
        void SetForeground(int color) const;

        void ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev) {
            ICompositeGlyph::ProcessEvent(w, p, ev);
        }
    private:
        friend EventManager;
//        class WindowImpl;
        std::unique_ptr<WindowImpl> m_window_impl;


    };
}

#endif // LEXI_WINDOW_HPP