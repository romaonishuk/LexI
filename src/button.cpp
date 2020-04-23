//
// Created by romaonishuk on 14.12.19.
//

#include "button.hpp"

#include "graphic_primitives.hpp"

/// ---- Button -------
Button::Button(GlyphParams params, const std::string& text): IGlyph(params)
{
    m_text = text;
}

void Button::Draw(Gui::Window* w)
{
    DrawButton(w, m_params);
    w->DrawText(m_params, m_text, Alignment::kCenter);
}

void Button::ProcessEvent(Gui::Window* w, const Event& event)
{
    switch(event.GetEvent()) {
        case EventType::MouseButtonPressed:
            DrawPressedButton(w, m_params);
            m_isPressed = true;
            break;
        case EventType::MouseButtonReleased:
            if(m_isPressed) {
                DrawButton(w, m_params);
            }

            m_isPressed = false;
            break;
        default:
            // TODO(rmn): add log
            return;
    }
}
