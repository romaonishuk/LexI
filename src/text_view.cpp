//
// Created by ronyshchuk on 07.04.20.
//

#include "text_view.hpp"
#include "cursor.hpp"
#include "font.hpp"
#include "page.hpp"
#include "window.hpp"

TextView::TextView(const GlyphParams& params, Gui::Window* w):
    Gui::Window(params, w),
    m_visibleArea(0, 0, params.width, params.height)
{
    const auto pageHeight = 200;
    m_currentPage = std::make_shared<Page>(this, GlyphParams{0, 0, m_params.width - 1, pageHeight});
    ICompositeGlyph::Add(m_currentPage);
    m_visiblePages.push_back(m_currentPage);
    this->ShowWindow();
}

void TextView::Draw(Gui::Window* window)
{
    for(auto& page: m_visiblePages) {
        page->Draw(this);
    }

    m_currentPage->DrawCursor(this);
}

// TODO(rmn): chunking
void TextView::ProcessEvent(Gui::Window* window, const Event& event)
{
    if(event.GetEvent() == EventType::KeyPressed) {
        m_currentPage->ProcessEvent(window, event);

        if(Lexi::Cursor::Get().GetCursorEnd() < m_visibleArea.y) {
            m_visibleArea.y -= Lexi::FontManager::Get().GetCharHeight();
            IGlyph::ReDraw(window);
        }

        if(Lexi::Cursor::Get().GetCursorEnd() > m_visibleArea.y + m_params.height) {
            m_visibleArea.y += Lexi::FontManager::Get().GetCharHeight();
            IGlyph::ReDraw(window);
        }

        return;
    }
}

void TextView::DrawRectangle(const GlyphParams& params) const
{
    if(m_visibleArea.IsIntersects(params) || params.IsIntersects(m_visibleArea)) {
        m_window_impl->DrawRectangle({params.x, params.y - m_visibleArea.y}, params.width, params.height);
    }
}

void TextView::DrawText(const Point& text_position, std::string text) const
{
    if(m_visibleArea.IsIntersects({text_position.x, text_position.y})) {
        m_window_impl->DrawText({text_position.x, text_position.y - m_visibleArea.y}, text);
    }
}

void TextView::DrawLine(const Point& start_point, const Point& end_point) const
{
    // TODO(rmn) :intersection
    m_window_impl->DrawLine(
        {start_point.x, start_point.y - m_visibleArea.y}, {end_point.x, end_point.y - m_visibleArea.y});
}

void TextView::UpdateVisibleArea(height_t h)
{
    m_visibleArea.y = h;
    Draw(this);
}

void TextView::MoveVisibleArea(height_t h)
{
    m_visibleArea.y += h;
    Draw(this);
}
