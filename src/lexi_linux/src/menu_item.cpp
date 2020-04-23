//
// Created by ronyshchuk on 30.03.20.
//

#include "menu_item.hpp"
#include "i_command.hpp"

#include "graphic_primitives.hpp"

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
    DrawButton(w, m_params);
    w->DrawText(m_params, m_text, Alignment::kLeft);
}

void MenuItem::ProcessEvent(Gui::Window* w, const Event& event)
{
    switch(event.GetEvent()) {
        case EventType::MouseButtonPressed:
            DrawPressedButton(w, m_params);

            if(m_onButtonPressed) {
                m_onButtonPressed();
            }

            break;
        case EventType::MouseButtonReleased:
            Draw(w);
            if(m_command) {
                m_command->Execute();
            }
            break;
        case EventType::MouseMotion:
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
