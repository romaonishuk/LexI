//
// Created by romaonishuk on 27.10.19.
//

#ifndef LEXI_WINDOW_HPP
#define LEXI_WINDOW_HPP

#include <memory>

#include "i_glyph.hpp"
#include "i_composite_glyph.hpp"
// TODO: WindowImpl should be private for Window
#include "window_impl.hpp"
#include "types.hpp"

#include "event_manager.hpp"

namespace Gui
{
    class ChildWindow;

    class Window : public ICompositeGlyph
    {
    public:
        explicit Window(const GlyphParams&);
        Window(const GlyphParams&, std::unique_ptr<WindowImpl>);
        ~Window() override;

        void Draw(Gui::Window *) override;
        void DrawRectangle(const Point& point, const width_t width, const height_t height) const;
        void DrawText(const Point& text_position, std::string text) const;
        void DrawLine(const Point& start_point, const Point& end_point) const;
        
        void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color);
        
        void SetForeground(int color) const;

        void ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev) override {
            ICompositeGlyph::ProcessEvent(w, p, ev);
        }

//    TODO(rmn): protected:
    public:
        friend EventManager;
//        class WindowImpl;
        std::unique_ptr<WindowImpl> m_window_impl;
    };

    class ChildWindow : public Window
    {
    public:
        ChildWindow(const GlyphParams&, Window*);
        void Draw(Window *) override;
        void Destroy();
    private:
        // TODO(rmn): weak_ptr
        Window* m_parent;
    };
}

#endif // LEXI_WINDOW_HPP
