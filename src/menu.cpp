//
// Created by romaonishuk on 04.03.20.
//

#include "menu.hpp"
#include "i_command.hpp"

#include <iostream>

// TODO(rmn): remove copy-paste

// TODO(rmn): Menu improvement: it width and heigth should be calculated relatively to MenuItems
namespace Gui {
Menu::Menu(const GlyphParams& p, const std::string& title, Window* w):
    ICompositeGlyph(p),
    mMenuWindow(std::make_unique<ChildWindow>(GlyphParams{p.x, p.y + p.height, p.width, p.height}, w)),
    m_title(title)
{}

void Menu::Draw(Window* w)
{
    // TODO(rmn): weird logic
    if(!m_components.empty()) {
        InitMenuWindow();
    }

    w->SetForeground(kBlack);

    w->DrawText({m_params.x + m_params.width, m_params.y + m_params.height}, m_title);
    w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
}

void Menu::ProcessEvent(Window* w, const Point& p, const EventType& ev)
{
    if(ev == EventType::ButtonPressed) {
        mMenuWindow->ShowWindow();
        std::cout << "Button pressed" << std::endl;
    } else if(ev == EventType::ButtonReleased) {
        std::cout << "Button ButtonReleased" << std::endl;
        mMenuWindow->HideWindow();
        std::cout << "Button ButtonReleased" << std::endl;
    }
}

void Menu::InitMenuWindow()
{
    const auto defaultHeight = 25;
    const auto defaultWidth = 200;

    width_t width = defaultWidth;

    unsigned int index = 0;
    for(auto &it: m_components) {
        it->SetGlyphParams({0, index*defaultHeight, defaultWidth, defaultHeight});
        mMenuWindow->Add(it);
        index++;
    }

    mMenuWindow->Resize(width, index*defaultHeight);
    m_components.clear();
}

}  // namespace Gui
