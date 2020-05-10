//
// Created by ronyshchuk on 09.05.20.
//

#include "character.hpp"

#include "cursor.hpp"
#include "font.hpp"
#include "window.hpp"

void Character::Draw(Gui::Window* window)
{
    window->SetForeground(kBlack);
    // calibrating position in respect to baseline  - Lexi::FontManager::Get().GetCharDescent()
    // TODO(rmn): fonting fonts!
    window->DrawText(
        {m_params.x, m_params.y + m_params.height - Lexi::FontManager::Get().GetCharDescent()}, {m_symbol});
}

void Character::DrawAt(Gui::Window* window, const Point& point)
{
    window->SetForeground(kBlack);
    window->DrawText({point.x, point.y + Lexi::FontManager::Get().GetCharAscent()}, {m_symbol});
}
