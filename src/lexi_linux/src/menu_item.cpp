//
// Created by ronyshchuk on 30.03.20.
//

#include "menu_item.hpp"
#include "i_command.hpp"
#include "window.hpp"

#include <iostream>

namespace Gui {
void MenuItem::SetCommand(std::unique_ptr<ICommand> cmd)
{
    m_command = std::move(cmd);
}

void MenuItem::SetOnFocusedAction(std::function<void()>&& f)
{
    m_onFocused = std::move(f);
}

void MenuItem::SetOnButtonPressedAction(std::function<void()>&& f)
{
    m_onButtonPressed = std::move(f);
}

void MenuItem::Draw(Window* w)
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

    w->DrawText(m_params, m_text, Alignment::kLeft);
}

void MenuItem::ProcessEvent(Gui::Window* w, const Event& event)
{
    switch(event.GetEvent()) {
        case EventType::ButtonPressed:
            w->SetForeground(Color::kWhite);

            // bottom
            w->DrawLine({m_params.x, m_params.y + m_params.height},
                {m_params.x + m_params.width, m_params.y + m_params.height});
            // right
            w->DrawLine(
                {m_params.x + m_params.width, m_params.y}, {m_params.x + m_params.width, m_params.y + m_params.height});

            w->SetForeground(Color::kBlack);

            // top
            w->DrawLine({m_params.x, m_params.y}, {m_params.x + m_params.width, m_params.y});
            // left
            w->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});

            w->SetForeground(Color::kGray);

            if(m_onButtonPressed) {
                m_onButtonPressed();
            }

            break;
        case EventType::ButtonReleased:
            Draw(w);
            if(m_command) {
                m_command->Execute();
            }
            break;
        case EventType::FocusedIn:
            w->FillRectangle(m_params, m_params.width, m_params.height, kLightBlue);
            w->SetForeground(Color::kBlack);
            w->DrawText(m_params, m_text, Alignment::kLeft);

            if(m_onFocused) {
                m_onFocused();
            }
            break;
        default:
            return;
    }
}
}  // namespace Gui
