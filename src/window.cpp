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
    Window::Window() : ICompositeGlyph(GlyphParams())
    {
        m_window_impl = WindowSystemFactory::Instance().MakeWindowImpl();
        assert(m_window_impl);
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

    int Window::GetEvent()
    {
        return m_window_impl->GetEvent();
    }

    void Window::ProcessEvent(int event_type)
    {
        for(auto &it: m_components) {
            if(auto ptr = dynamic_cast<Button*>(it.get())) {
                if(event_type == 4) {
                    ptr->OnButtonPressed(this);
                }
                else if(event_type == 5) {
                    ptr->OnButtonReleased(this);
                }
            }
        }
    }

}
