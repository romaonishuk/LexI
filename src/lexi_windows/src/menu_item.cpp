//
// Created by romaonishuk on 03.12.2020.
//

#include "menu_item.hpp"
#include "i_command.hpp"

// TODO(rmn): reconsider necessity of this file. I suppose it should be a window.

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
}

void MenuItem::ProcessEvent(Gui::Window* w, const Event& event)
{
}
}  // namespace Gui
