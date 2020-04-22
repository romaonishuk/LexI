//
// Created by romaonishuk on 14.12.19.
//

#include "button.hpp"

#include "window.hpp"

/// ---- Button -------
Button::Button(GlyphParams params, const std::string& text): IGlyph(params)
{
    m_text = text;
}

void Button::Draw(Gui::Window* w)
{
    w->SetForeground(Color::kWhite);

    // top
    w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
    // left
    w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

    w->SetForeground(Color::kBlack);

    // bottom
    w->DrawLine(
        {m_params.x, m_params.y + m_params.height}, {m_params.x + m_params.width, m_params.y + m_params.height});

    // right
    w->DrawLine({m_params.x + m_params.width, m_params.y}, {m_params.x + m_params.width, m_params.y + m_params.height});

    w->DrawText(m_params, m_text, Alignment::kCenter);
}

void Button::ProcessEvent(Gui::Window* w, const Event& event)
{
    switch(event.GetEvent()) {
        case EventType::MouseButtonPressed:
            OnButtonPressed(w);
            m_isPressed = true;
            break;
        case EventType::MouseButtonReleased:
            if(m_isPressed) {
                OnButtonReleased(w);
            }

            m_isPressed = false;
            break;
        default:
            // TODO(rmn): add log
            return;
    }
}

void Button::OnButtonPressed(Gui::Window* w)
{
    w->SetForeground(Color::kWhite);

    // bottom
    w->DrawLine(
        {m_params.x, m_params.y + m_params.height}, {m_params.x + m_params.width, m_params.y + m_params.height});
    // right
    w->DrawLine({m_params.x + m_params.width, m_params.y}, {m_params.x + m_params.width, m_params.y + m_params.height});

    w->SetForeground(Color::kBlack);

    // top
    w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
    // left
    w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

    w->SetForeground(Color::kGray);
}

void Button::OnButtonReleased(Gui::Window* w)
{
    Draw(w);
}
