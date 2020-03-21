//
// Created by romaonishuk on 28.10.19.
//

#include "window.hpp"
#include "window_impl.hpp"
#include "window_system_factory.hpp"
#include <cassert>
#include "button.hpp"

#include "logger.hpp"

namespace Gui
{
    Window::Window(const GlyphParams& params) : ICompositeGlyph(params)
    {
        m_window_impl = WindowSystemFactory::Get().MakeWindowImpl(params);
        assert(m_window_impl);
    }

    Window::Window(const GlyphParams& params, std::unique_ptr<WindowImpl> impl):ICompositeGlyph(params),
        m_window_impl(std::move(impl))
    {

    }

    Window::~Window() = default;

    void Window::Draw(Gui::Window *window)
    {
//        m_window_impl->Redraw()
        ICompositeGlyph::Draw(window);
    }

    void Window::SetForeground(int color) const
    {
        m_window_impl->SetForeground(color);
    }

    void Window::DrawRectangle(const Point& point, const width_t width, const height_t heigth) const
    {
        m_window_impl->DrawRectangle(point, width, heigth);
    }

    void Window::DrawText(const Point& text_position, std::string text) const
    {
        m_window_impl->DrawText(text_position, text);
    }

    void Window::DrawLine(const Point& start_point, const Point& end_point) const
    {
        m_window_impl->DrawLine(start_point, end_point);
    }

    void Window::FillRectangle(const Point& point, const width_t width, const height_t height, const Color color)
    {
        m_window_impl->FillRectangle(point, width, height, color);
    }

// --- ChildWindow ---
    ChildWindow::ChildWindow(const GlyphParams& params, Window* parent):
        Window(params, WindowSystemFactory::Get().MakeChildWindowImpl(params, parent->m_window_impl.get())),
        m_parent(parent)
    {
    }

    void ChildWindow::Draw(Gui::Window *) {
        m_window_impl->ShowWindow();
    }

    void ChildWindow::Destroy() {
        m_window_impl->Destroy();
    }
}
