//
// Created by ronyshchuk on 30.03.20.
//

#ifndef LEXI_MENU_ITEM_HPP
#define LEXI_MENU_ITEM_HPP

#include <functional>
#include "i_glyph.hpp"

class ICommand;

namespace Gui {
class MenuItem: public IGlyph
{
public:
    explicit MenuItem(const std::string& text): IGlyph(GlyphParams{}), m_text(text) {}

    void Draw(Window* w) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override;

    void SetCommand(std::unique_ptr<ICommand>);
    void SetOnFocusedAction(std::function<void()>&&);
    void SetOnButtonPressedAction(std::function<void()>&&);

    [[nodiscard]] const std::string& GetText() const { return m_text; }

private:
    std::unique_ptr<ICommand> m_command;
    std::string m_text;

    std::function<void()> m_onFocused;
    std::function<void()> m_onButtonPressed;
};
}  // namespace Gui
#endif  // LEXI_MENU_ITEM_HPP
