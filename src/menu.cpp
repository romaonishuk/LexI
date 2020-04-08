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

    w->DrawText(m_params, m_title, Alignment::kCenter);
    w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
}

void Menu::ProcessEvent(Gui::Window* w, const Event& event)
{
    const auto& ev = event.GetEvent();
    if(ev == EventType::ButtonPressed) {
        mMenuWindow->ShowWindow();
    } else if(ev == EventType::ButtonReleased) {
        mMenuWindow->HideWindow();
    }
}

void Menu::InitMenuWindow()
{
    const auto defaultHeight = 25;
    const auto defaultWidth = 200;

    width_t width = defaultWidth;

    unsigned int index = 0;
    for(auto& it: m_components) {
        it->SetGlyphParams({0, index * defaultHeight, defaultWidth, defaultHeight});
        mMenuWindow->Add(it);
        index++;
    }

    mMenuWindow->Resize(width, index * defaultHeight);
    m_components.clear();
}

// --- DropDownMenu ---

void DropDownMenu::Draw(Window* w)
{
    // TODO(rmn): weird logic
    if(!m_components.empty()) {
        InitMenuWindow();
    }

    w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
    w->FillRectangle({m_params.x, m_params.y}, m_params.width, m_params.height, Color::kWhite);

    w->SetForeground(Color::kBlack);
    w->DrawText(m_params, m_title, Alignment::kLeft);
}
}  // namespace Gui
