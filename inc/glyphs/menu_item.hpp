//
// Created by ronyshchuk on 30.03.20.
//

#ifndef LEXI_MENU_ITEM_HPP
#define LEXI_MENU_ITEM_HPP

#include "i_glyph.hpp"

class ICommand;

namespace Gui {
class MenuItem: public IGlyph
{
public:
    explicit MenuItem(const GlyphParams params, const std::string& text): IGlyph(params), m_text(text) {}

    void Draw(Window* w) override;

    void ProcessEvent(Window* w, const Point& p, const EventType& ev) override;

    void SetCommand(std::unique_ptr<ICommand>);

private:
    std::unique_ptr<ICommand> m_command;
    std::string m_text;
};
}  // namespace Gui
#endif  // LEXI_MENU_ITEM_HPP
