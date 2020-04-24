//
// Created by romaonishuk on 04.03.20.
//

#include "menu.hpp"
#include "i_command.hpp"

#include "graphic_primitives.hpp"

namespace Gui {

// --- MenuWindow ---
MenuWindow::MenuWindow(const GlyphParams& params, Window* parent): Window(params, parent), m_parent(parent)
{
    m_hideOnWindowSwitch = true;
    m_isVisible = false;
}

void MenuWindow::Draw([[maybe_unused]] Gui::Window* w)
{
    m_window_impl->SetForeground(kBlack);
    Gui::Window::Draw(this);
}

void MenuWindow::Resize(width_t width, height_t height)
{
    m_window_impl->Resize(width, height);
}

void MenuWindow::ProcessEvent(Gui::Window* w, const Event& event)
{
    for(const auto& it: m_components) {
        if(it->Intersects(event.GetPoint())) {
            // Menu item hasn't changed
            if(event.GetEvent() == EventType::MouseMotion && m_currentMenuItem) {
                if(it == m_currentMenuItem) {
                    return;
                }

                m_currentMenuItem->ReDraw(this);
            }

            SetCurrentMenuItem(it);
            return it->ProcessEvent(w, event);
        }
    }
}

// --- Menu ---

Menu::Menu(const GlyphParams& p, const std::string& title, Window* w):
    ICompositeGlyph(p),
    mMenuWindow(std::make_unique<MenuWindow>(GlyphParams{p.x, p.y + p.height, p.width, p.height}, w)),
    m_title(title)
{}

void Menu::Draw(Window* w)
{
    // TODO(rmn): weird logic
    if(!m_components.empty()) {
        InitMenuWindow();
    }

    DrawButton(w, m_params);
    w->DrawText(m_params, m_title, Alignment::kCenter);
}

void Menu::ProcessEvent(Gui::Window* w, const Event& event)
{
    const auto& ev = event.GetEvent();
    if(ev == EventType::MouseButtonPressed) {
        if(mMenuWindow->IsVisible()) {
            DrawButton(w, m_params);
            mMenuWindow->HideWindow();
        } else {
            DrawPressedButton(w, m_params);
            mMenuWindow->ShowWindow();
        }
    }
}

void Menu::InitMenuWindow()
{
    const auto defaultHeight = 25;
    const auto defaultWidth = 200;

    width_t width = defaultWidth;

    int index = 0;
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
