//
// Created by romaonishuk on 04.03.20.
//
#include "i_composite_glyph.hpp"
#include "window.hpp"
#include "menu.hpp"

// --- MenuItem ---
namespace Gui {
    Menu::Menu(const GlyphParams &p) : ICompositeGlyph(p) {
    }

    void Menu::Draw(Window *w) {
        w->SetForeground(kBlack);
        w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
    }

    void Menu::ProcessEvent(Window *w, const Point &p, const EventType &ev) {
        if (ev == EventType::ButtonPressed) {
            // TODO(rmn): needs improvement
            mMenuWindow = std::make_unique<ChildWindow>(m_params, w);
            mMenuWindow->Draw(w);
        } else if (ev == EventType::ButtonReleased) {
            mMenuWindow.reset();
        }
    }
}
