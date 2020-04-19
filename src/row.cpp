//
// Created by ronyshchuk on 08.04.20.
//

#include "row.hpp"

#include <algorithm>

#include "cursor.hpp"
#include "font.hpp"
#include "window.hpp"

constexpr auto cursorWidth = 1;

auto& cursor = Lexi::Cursor::Get();

// --- Character ---
void Character::Draw(Gui::Window* window)
{
    window->SetForeground(kBlack);
    // calibrating position in respect to baseline  - Lexi::FontManager::Get().GetCharDescent()
    // TODO(rmn): fonting fonts!
    window->DrawText({m_params.x, m_params.y}, {m_symbol});
}

void Character::DrawAt(Gui::Window* window, const Point& point)
{
    window->SetForeground(kBlack);
    window->DrawText({point.x, point.y + Lexi::FontManager::Get().GetCharAscent()}, {m_symbol});
}

// --- Row ---

Row::Row(const GlyphParams& params): ICompositeGlyph(params)
{}

bool Row::IsFull() const
{
    return m_usedWidth + Lexi::FontManager::Get().GetCharWidth() >= m_params.width;
}

void Row::AddCharacter(Gui::Window* w, char c)
{
    const auto& fontManager = Lexi::FontManager::Get();
    const auto charWidth = fontManager.GetCharWidth();
    const auto charHeight = fontManager.GetCharHeight();

    uint32_t newCharPos = m_params.x;
    if(!m_components.empty()) {
        const auto& lastChar = GetLast();
        newCharPos = lastChar->GetPosition().x + lastChar->GetWidth();
    }

    auto newChar = std::make_shared<Character>(GlyphParams{newCharPos, m_params.y, charWidth, charHeight}, c);
    Add(newChar);
    newChar->Draw(w);
    cursor.MoveCursor(w, {newCharPos + charWidth, m_params.y, cursorWidth, m_params.height});
    m_usedWidth += newChar->GetWidth();
}

void Row::Draw(Gui::Window* window)
{
    window->SetForeground(Color::kBlack);
    ICompositeGlyph::Draw(window);
}

void Row::DrawCursor(Gui::Window* window)
{
    auto cursorXPosition = cursor.GetPosition().x;
    if(m_components.empty() || cursorXPosition == m_params.x) {
        cursor.MoveCursor(window, {m_params.x, m_params.y, cursorWidth, m_params.height});
        return;
    }

    const auto& lastChar = m_components.back();
    if(cursorXPosition > lastChar->GetPosition().x + lastChar->GetWidth()) {
        cursor.MoveCursor(
            window, {lastChar->GetPosition().x + lastChar->GetWidth(), m_params.y, cursorWidth, m_params.height});
    }

    auto cursorPosition = Point(cursorXPosition - cursorWidth, m_params.y);
    for(auto& it: m_components) {
        if(it->Intersects(cursorPosition)) {
            cursor.MoveCursor(
                window, {it->GetGlyphParams().x + it->GetWidth(), m_params.y, cursorWidth, m_params.height});
            return;
        }
    }
}

IGlyph::GlyphPtr Row::GetLast() const
{
    return m_components.empty() ? nullptr : m_components.back();
}

void Row::Remove(Gui::Window* w, const GlyphPtr& ptr)
{
    if(ptr) {
        m_components.remove(ptr);
        ReDraw(w);
        DrawCursor(w);
    }
}

void Row::ProcessEvent(Gui::Window* window, const Event& event)
{
    if(event.GetEvent() != Lexi::EventType::KeyPressed) {
        return;
    }

    const auto* key = static_cast<const Lexi::KeyBoardEvent*>(&event);
    auto cursorPosition = Point(cursor.GetPosition().x, m_params.y);
    auto currentGlyph = std::find_if(
        m_components.begin(), m_components.end(), [&](const auto& c) { return c->Intersects(cursorPosition); });

    if(currentGlyph == m_components.end()) {
        std::cout << "Error! Cursor doesn't belong to this row! " << std::endl;
        //            cursor.MoveCursor(window,
        return;
    }

    if(key->GetKey() == Lexi::Key::kArrowLeft) {
        if(currentGlyph == m_components.begin()) {
            cursor.MoveCursor(window, {m_params.x, m_params.y, cursorWidth, m_params.height});
        } else {
            const auto& prevGlyph = *(std::prev(currentGlyph));
            cursor.MoveCursor(window,
                {prevGlyph->GetGlyphParams().x + prevGlyph->GetWidth(), m_params.y, cursorWidth, m_params.height});
        }
    } else if(key->GetKey() == Lexi::Key::kArrowRight) {
        const auto& nextGlyph = *(std::next(currentGlyph));
        cursor.MoveCursor(
            window, {nextGlyph->GetGlyphParams().x + nextGlyph->GetWidth(), m_params.y, cursorWidth, m_params.height});
    }
}
