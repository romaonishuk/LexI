//
// Created by romaonishuk on 14.12.19.
//

#include "button.hpp"

#include "window.hpp"

// TODO(rmn): tempo
#include <iostream>

/// ---- Button -------
Button::Button(GlyphParams params, std::string text) : IGlyph(params)
{
    m_text = text;
}

void Button::Draw(Gui::Window *w)
{
    w->SetForeground(Color::kWhite);

    // top
    w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
    // left
    w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

    w->SetForeground(Color::kBlack);

    // bottom
    w->DrawLine({m_params.x, m_params.y + m_params.height},
                {m_params.x + m_params.width, m_params.y + m_params.height});

    // right
    w->DrawLine({m_params.x + m_params.width, m_params.y},
                {m_params.x + m_params.width, m_params.y + m_params.height});


    // TODO: font should be included
    w->DrawText({m_params.x + m_params.width/2, m_params.y + m_params.height/2}, m_text);
}

void Button::ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev)
{
    switch(ev)
    {
        case EventType::ButtonPressed:
            OnButtonPressed(w);
            break;
        case EventType::ButtonReleased:
            OnButtonReleased(w);
            break;
        default:
            std::cout << "Button received event:" << static_cast<int>(ev) << std::endl;
            return;
    }
}


void Button::OnButtonPressed(Gui::Window *w) 
{
    w->SetForeground(Color::kWhite);

    // bottom
    w->DrawLine({m_params.x, m_params.y + m_params.height},
                {m_params.x + m_params.width, m_params.y + m_params.height});
    // right
    w->DrawLine({m_params.x + m_params.width, m_params.y},
                {m_params.x + m_params.width, m_params.y + m_params.height});

    w->SetForeground(Color::kBlack);

    // top
    w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
    // left
    w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

    w->SetForeground(Color::kGray);
}

void Button::OnButtonReleased(Gui::Window *w)
{
    Draw(w);
}
