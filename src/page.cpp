//
// Created by ronyshchuk on 19.04.20.
//

#include "page.hpp"

#include <algorithm>

#include "cursor.hpp"
#include "font.hpp"
#include "row.hpp"
#include "text_view.hpp"

Page::Page(TextView* parent, const GlyphParams& params): ICompositeGlyph(params), m_parent(parent)
{
    m_currentRow = std::make_shared<Row>(
        GlyphParams{m_leftIndent, m_params.y + m_topIndent + Lexi::FontManager::Get().GetCharHeight(),
            m_params.width - m_leftIndent - m_rightIndent, Lexi::FontManager::Get().GetCharHeight()});
    ICompositeGlyph::Add(m_currentRow);
}

void Page::Draw(Gui::Window* window)
{
    window->FillRectangle(m_params, Color::kWhite);
    window->SetForeground(Color::kBlack);
    window->DrawRectangle(m_params);

    ICompositeGlyph::Draw(window);
}

void Page::DrawCursor(Gui::Window* window)
{
    m_currentRow->DrawCursor(window);
}

void Page::ProcessEvent(Gui::Window* window, const Event& event)
{
    // TODO(rmn): oh my, need to add event/notification mechanism
    const auto* key = static_cast<const Lexi::KeyBoardEvent*>(&event);

    if(key->IsString()) {
        if(!m_currentRow->IsFull()) {
            m_currentRow->AddCharacter(window, key->m_key);
        } else {
            // TODO(rmn): add new row. and add character
        }
    }

    switch(static_cast<Lexi::Key>(key->m_key)) {
        case Lexi::Key::kEnter: {
            auto fontHeight = Lexi::FontManager::Get().GetCharHeight();
            if(m_currentRow->GetPosition().y + fontHeight < m_params.y + m_params.height - m_botIndent) {
                m_currentRow = std::make_shared<Row>(
                    GlyphParams{m_params.x + m_leftIndent, m_currentRow->GetPosition().y + fontHeight,
                        m_params.width - m_leftIndent - m_rightIndent, fontHeight});
                Add(m_currentRow);
                m_currentRow->DrawCursor(window);
            } else {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kNext, true);
            }
        } break;
        case Lexi::Key::kArrowUp: {
            if(m_currentRow == m_components.front()) {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kPrev, true);
            } else {
                auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
                m_currentRow = std::static_pointer_cast<Row>(*(std::prev(rowIt)));
                m_currentRow->DrawCursor(window);
            }
        } break;
        case Lexi::Key::kArrowDown: {
            if(m_currentRow == m_components.back()) {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kNext, false);
            } else {
                auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
                m_currentRow = std::static_pointer_cast<Row>(*(std::next(rowIt)));
                m_currentRow->DrawCursor(window);
            }
        } break;
        case Lexi::Key::kArrowLeft: {
            if(Lexi::Cursor::Get().GetPosition().x == m_currentRow->GetPosition().x) {
                if(m_currentRow == m_components.front()) {
                    // switch page
                    // and move cursor!
                } else {
                    // TODO(rmn): draw at the end!
                    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
                    m_currentRow = std::static_pointer_cast<Row>(*std::prev(rowIt));
                    m_currentRow->DrawCursor(window);
                }
            } else {
                m_currentRow->ProcessEvent(window, event);
            }
        } break;
        case Lexi::Key::kArrowRight: {
            auto lastChar = m_currentRow->GetLast();
            if(lastChar && lastChar->GetPosition().x + lastChar->GetWidth() == Lexi::Cursor::Get().GetPosition().x) {
                if(m_currentRow == m_components.back()) {
                    // switch page
                } else {
                    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
                    m_currentRow = std::static_pointer_cast<Row>(*std::next(rowIt));
                    m_currentRow->DrawCursor(window);
                }
            } else {
                m_currentRow->ProcessEvent(window, event);
            }
        } break;
        case Lexi::Key::kBackspace:
            m_currentRow->Remove(window, m_currentRow->GetLast());
            break;
        default:
            break;
    }
}
