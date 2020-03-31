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

    // TODO(rmn): font should be included
    constexpr auto hMagic = 5;
    constexpr auto wMagic = 10;

    w->DrawText({m_params.x + m_params.width / 2 - wMagic, m_params.y + m_params.height / 2 + hMagic}, m_text);
}

void Button::ProcessEvent(Gui::Window* w, const Point& p, const EventType& ev)
{
    switch(ev) {
        case EventType::ButtonPressed:
            OnButtonPressed(w);
            break;
        case EventType::ButtonReleased:
            OnButtonReleased(w);
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
