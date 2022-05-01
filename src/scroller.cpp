//
// Created by ronyshchuk on 21.04.20.
//

#include "scroller.hpp"

#include "button.hpp"
#include "text_view.hpp"

Scroller::Scroller(const GlyphParams& p, std::shared_ptr<TextView> view): IGlyph(p), m_view(std::move(view))
{
    m_scale = m_params.height / static_cast<float>(m_view->GetOverallHeight());
    m_scrollButton = std::make_shared<Button>(GlyphParams{m_params.x + 1, m_params.y, m_params.width - 2, 100}, "");
    SetButtonSize();
}

void Scroller::Draw(Gui::Window* window)
{
    window->SetForeground(kBlack);
    window->DrawRectangle(m_params);

    m_scrollButton->Draw(window);
}

void Scroller::ProcessEvent(Gui::Window* w, const Event& event)
{
    const auto& ev = event.GetEvent();
    if(ev == Lexi::EventType::MouseMotion && m_scrollButton->IsPressed()) {
        auto& eventPosition = event.GetPoint();
        auto buttonParams = m_scrollButton->GetGlyphParams();

        height_t delta = eventPosition.y - m_previousPosition.y;
        buttonParams.y += delta;
        if(buttonParams.y < m_params.y || buttonParams.y + buttonParams.height > m_params.y + m_params.height) {
            return;
        }

        // TODO(rmn): redraw only button
        ClearGlyph(w);
        m_scrollButton->SetGlyphParams(buttonParams);
        Draw(w);

        // NOTE: since we assume that scrollBoard begin is equal TextView begin, we need  to subtract m_params.y
        m_view->UpdateVisibleArea((buttonParams.y - m_params.y) / m_scale);
        m_previousPosition = event.GetPoint();
    } else if(ev == Lexi::EventType::Scroll) {
        const auto* scrollEvent = static_cast<const Lexi::ScrollEvent*>(&event);
        auto buttonParams = m_scrollButton->GetGlyphParams();

        if(scrollEvent->GetDirection() == Lexi::ScrollEvent::Direction::kUp) {
            if(buttonParams.y == m_params.y) {
                return;
            }
            buttonParams.y -= scrollEvent->GetScrolledDistance();
            // roll less than 15 px
            if(buttonParams.y < m_params.y) {
                buttonParams.y = m_params.y;
            }
        } else {
            if(m_scrollButton->GetBottomBorder() == GetBottomBorder()) {
                return;
            }

            // roll less than 15 px
            buttonParams.y += scrollEvent->GetScrolledDistance();
            if(buttonParams.y + buttonParams.height > GetBottomBorder()) {
                buttonParams.y = GetBottomBorder() - buttonParams.height;
            }
        }

        // TODO(rmn): redraw only button
        ClearGlyph(w);
        m_scrollButton->SetGlyphParams(buttonParams);
        Draw(w);

        // NOTE: since we assume that scrollBoard begin is equal TextView begin, we need  to subtract m_params.y
        m_view->UpdateVisibleArea((buttonParams.y - m_params.y) / m_scale);
    } else if(m_scrollButton->Intersects(event.GetPoint())) {
        if(ev == Lexi::EventType::MouseButtonPressed) {
            m_previousPosition = event.GetPoint();
        }
        // TODO(rmn): thiiiiink
        m_scrollButton->ProcessEvent(w, event);
    }
}

void Scroller::UpdateScaling()
{
    m_scale = m_params.height / static_cast<float>(m_view->GetOverallHeight());
    SetButtonSize();
}

void Scroller::UpdateScrollerPosition(Gui::Window* w)
{
    m_scrollButton->SetPosition({m_scrollButton->GetPosition().x, static_cast<int32_t>(m_params.y + m_view->GetVisibleArea().y * m_scale)});
    ReDraw(w);
}

void Scroller::SetButtonSize()
{
    auto buttonParams = m_scrollButton->GetGlyphParams();
    if(m_scale > 1) {
        buttonParams.height = m_params.height;
    } else {
        buttonParams.height = m_params.height * m_scale;
        if(buttonParams.height == 0) {
            buttonParams.height = 1;
        }
    }

    m_scrollButton->SetGlyphParams(buttonParams);
}
