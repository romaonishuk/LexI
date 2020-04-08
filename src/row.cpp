//
// Created by ronyshchuk on 08.04.20.
//

#include "row.hpp"

#include "text_view.hpp"
#include "window.hpp"

// --- Character ---
void Character::Draw(Gui::Window* window)
{
    window->SetForeground(kBlack);
    window->DrawText({m_params.x, m_params.y}, {m_symbol});
}

// --- Row ---

Row::Row(TextView* parent, const GlyphParams& params):
    ICompositeGlyph(params),
    m_parent(parent),
    m_currentCharPosition(params.x)
{
    std::cout << params << std::endl;
}

uint32_t Row::GetLastCharPosition()
{
    if(m_components.empty()) {
        return m_params.x;
    }
    return m_components.back()->GetPosition().x;
}

void Row::AddCharacter(Gui::Window* w, char c)
{
    // TODO(rmn): work with fonts!
    constexpr auto charWidth = 10;
    constexpr auto charHeight = 25;

    if(m_currentCharPosition - m_params.x + charWidth > m_params.width) {
        m_parent->AddNewRow(w, c);
    } else {
        auto newChar =
            std::make_shared<Character>(GlyphParams{m_currentCharPosition, m_params.y, charWidth, charHeight}, c);
        Add(newChar);
        newChar->Draw(w);
        m_currentCharPosition += newChar->GetWidth();
    }
}

void Row::Draw(Gui::Window* window)
{
    for(auto& it: m_components) {
        it->SetPosition({it->GetPosition().x, m_params.y});
        it->Draw(window);
    }
}
