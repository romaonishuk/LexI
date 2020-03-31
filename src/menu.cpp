//
// Created by romaonishuk on 04.03.20.
//

#include "menu.hpp"
#include "i_command.hpp"

// TODO(rmn): remove copy-paste

// TODO(rmn): Menu improvement: it width and heigth should be calculated relatively to MenuItems
namespace Gui {
Menu::Menu(const GlyphParams& p, const std::string& title, Window* w):
    ICompositeGlyph(p),
    mMenuWindow(std::make_unique<ChildWindow>(GlyphParams{p.x, p.y + p.height, 200, 50}, w)),
    m_title(title)
{}

void Menu::Draw(Window* w)
{
    w->SetForeground(kBlack);
    constexpr auto hMagic = 5;
    constexpr auto wMagic = 10;

    w->DrawText({m_params.x + m_params.width / 2 - wMagic, m_params.y + m_params.height / 2 + hMagic}, m_title);
    w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
}

void Menu::ProcessEvent(Window* w, const Point& p, const EventType& ev)
{
    if(ev == EventType::ButtonPressed) {
        for(auto& it: m_components) {
            mMenuWindow->Add(it);
        }
        mMenuWindow->ShowWindow();
    } else if(ev == EventType::ButtonReleased) {
        mMenuWindow->HideWindow();
    }
}

void Menu::Add(GlyphPtr glyph)
{
    mMenuWindow->Add(std::move(glyph));
}
}  // namespace Gui
