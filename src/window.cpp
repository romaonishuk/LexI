//
// Created by romaonishuk on 28.10.19.
//

#include "window.hpp"
#include <cassert>
#include "button.hpp"
#include "window_impl.hpp"
#include "window_system_factory.hpp"

#include "logger.hpp"

namespace Gui {
Window::Window(const GlyphParams& params): ICompositeGlyph(params)
{
    m_window_impl = WindowSystemFactory::Get().MakeWindowImpl(params);
    assert(m_window_impl);
}

Window::Window(const GlyphParams& params, Window* parentWindow):
    ICompositeGlyph(params),
    m_window_impl(WindowSystemFactory::Get().MakeChildWindowImpl(params, parentWindow->m_window_impl.get()))
{}

Window::~Window() = default;

void Window::Draw(Gui::Window* window)
{
    //        m_window_impl->Redraw()
    ICompositeGlyph::Draw(window);
}

void Window::ReDraw()
{
    m_window_impl->ClearWindow();
    Gui::Window::Draw(this);
}

void Window::SetForeground(int color) const
{
    m_window_impl->SetForeground(color);
}

void Window::DrawRectangle(const Point& point, const width_t width, const height_t height) const
{
    m_window_impl->DrawRectangle(point, width, height);
}

void Window::DrawRectangle(const GlyphParams& params) const
{
    m_window_impl->DrawRectangle({params.x, params.y}, params.width, params.height);
}

void Window::DrawText(const Point& text_position, std::string text) const
{
    m_window_impl->DrawText(text_position, text);
}

void Window::DrawText(const GlyphParams& params, const std::string& text, Alignment alignment)
{
    m_window_impl->DrawText(params, text, alignment);
}

void Window::DrawLine(const Point& start_point, const Point& end_point) const
{
    m_window_impl->DrawLine(start_point, end_point);
}

void Window::FillRectangle(const Point& point, const width_t width, const height_t height, const Color color)
{
    m_window_impl->FillRectangle(point, width, height, color);
}

void Window::FillRectangle(const GlyphParams& params, const Color color)
{
    m_window_impl->FillRectangle({params.x, params.y}, params.width, params.height, color);
}

void Window::ShowWindow() const
{
    m_window_impl->ShowWindow();
}

void Window::HideWindow() const
{
    m_window_impl->HideWindow();
}

// --- ChildWindow ---
ChildWindow::ChildWindow(const GlyphParams& params, Window* parent): Window(params, parent), m_parent(parent)
{}

void ChildWindow::Draw([[maybe_unused]] Gui::Window* w)
{
    m_window_impl->SetForeground(kBlack);
    Gui::Window::Draw(this);
}

void ChildWindow::Resize(width_t width, height_t height)
{
    m_window_impl->Resize(width, height);
}

void ChildWindow::ProcessEvent(Gui::Window* w, const Event& event)
{
    for(const auto& it: m_components) {
        if(it->Intersects(event.GetPoint())) {
            // Menu item hasn't changed
            if(event.GetEvent() == EventType::FocusedIn && m_currentMenuItem) {
                if(it == m_currentMenuItem) {
                    return;
                }

                m_currentMenuItem->ReDraw(this);
            }
            SetCurrentMenuItem(it);
            return it->ProcessEvent(w, event);
        }
    }
}

}  // namespace Gui
