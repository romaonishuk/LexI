//
// Created by ronyshchuk on 19.04.20.
//

#include "page.hpp"

#include <algorithm>

#include "character.hpp"
#include "cursor.hpp"
#include "font.hpp"
#include "row.hpp"
#include "text_view.hpp"

height_t Page::m_topIndent = 50;
height_t Page::m_botIndent = 25;
width_t Page::m_leftIndent = 50;
width_t Page::m_rightIndent = 75;

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

void Page::ProcessCharacterShift(std::shared_ptr<Row>& row, IGlyph::GlyphPtr& newChar)
{
    auto& cursor = Lexi::Cursor::Get();
    bool updateCursorLocation = false;
    // last char
    if(newChar->GetRightBorder() == cursor.GetPosition().x) {
        updateCursorLocation = true;
    }

    std::shared_ptr<Row> nextRow;
    std::shared_ptr<Page> nextPage;
    if(row == m_components.back()) {
        if(IsLastRow(row)) {
            nextPage = m_parent->GetNextPage(this);
            if(!nextPage) {
                nextPage = m_parent->AddPage(Lexi::Cursor::Get().GetCurrentWindow(), this);
            }
            nextRow = nextPage->GetFirstRow();
            nextPage->SetCurrentRow(nextRow);
        } else {
            nextRow = std::make_shared<Row>(
                GlyphParams{m_leftIndent, row->GetPosition().y + Lexi::FontManager::Get().GetCharHeight(),
                    m_params.width - m_leftIndent - m_rightIndent, Lexi::FontManager::Get().GetCharHeight()});
            ICompositeGlyph::Add(nextRow);
        }
    } else {
        nextRow = GetNextRow();
    }

    auto res = nextRow->AddCharacter(nextRow->GetPosition(), newChar);
    if(updateCursorLocation) {
        if(nextPage) {
            m_parent->SetCurrentPage(nextPage);
        } else {
            m_currentRow = nextRow;
        }

        auto newParams = newChar->GetGlyphParams();
        newParams.x = newChar->GetRightBorder();
        cursor.MoveCursor(cursor.GetCurrentWindow(), newParams);
    }

    if(res) {
        ProcessCharacterShift(nextRow, *res);
    }
}

void Page::ProcessEvent(Gui::Window* window, const Event& event)
{
    // TODO(rmn): oh my, need to add event/notification mechanism
    const auto* key = static_cast<const Lexi::KeyBoardEvent*>(&event);
    auto& cursor = Lexi::Cursor::Get();

    if(key->IsString()) {
        // new character add caused removing of last char from row
        if(auto res = m_currentRow->AddCharacter(key->m_key)) {
            ProcessCharacterShift(m_currentRow, *res);
            return;
        }
    }

    switch(static_cast<Lexi::Key>(key->m_key)) {
        case Lexi::Key::kEnter: {
            auto fontHeight = Lexi::FontManager::Get().GetCharHeight();
            if(!IsLastRow(m_currentRow)) {
                m_currentRow = std::make_shared<Row>(
                    GlyphParams{m_params.x + m_leftIndent, m_currentRow->GetPosition().y + fontHeight,
                        m_params.width - m_leftIndent - m_rightIndent, fontHeight});
                Add(m_currentRow);
                m_currentRow->DrawCursor(window);
            } else {
                auto nextPage = m_parent->SwitchPage(window, TextView::SwitchDirection::kNext, true);
                //                nextPage->Ad
            }
        } break;
        case Lexi::Key::kArrowUp: {
            if(m_currentRow == m_components.front()) {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kPrev, true);
            } else {
                m_currentRow = GetPreviousRow();
                m_currentRow->DrawCursor(window);
            }
        } break;
        case Lexi::Key::kArrowDown: {
            if(m_currentRow == m_components.back()) {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kNext, false);
            } else {
                m_currentRow = GetNextRow();
                m_currentRow->DrawCursor(window);
            }
        } break;
        case Lexi::Key::kArrowLeft: {
            if(cursor.GetPosition().x == m_currentRow->GetPosition().x) {
                // switch page
                if(m_currentRow == m_components.front()) {
                } else {  // move cursor to previous row
                    m_currentRow = GetPreviousRow();
                    m_currentRow->DrawCursorAtEnd(window);
                }
            } else {
                m_currentRow->ProcessEvent(window, event);
            }
        } break;
        case Lexi::Key::kArrowRight: {
            auto lastChar = m_currentRow->GetLastChar();
            if((lastChar && lastChar->GetRightBorder() == cursor.GetPosition().x) || m_currentRow->IsEmpty()) {
                if(m_currentRow == m_components.back()) {
                    // switch page
                } else {
                    m_currentRow = GetNextRow();
                    cursor.MoveCursor(window, m_currentRow->GetGlyphParams());
                }
            } else {
                m_currentRow->ProcessEvent(window, event);
            }
        } break;
        case Lexi::Key::kBackspace:
            if(Lexi::Cursor::Get().GetPosition().x == m_currentRow->GetPosition().x) {
                ProcessBackspace(window);
            } else {
                m_currentRow->ProcessEvent(window, event);
            }
            break;
        default:
            break;
    }
}

std::shared_ptr<Row> Page::GetFirstRow()
{
    return std::static_pointer_cast<Row>(m_components.front());
}

bool Page::IsLastRow(const GlyphPtr& row) const
{
    return row->GetBottomBorder() + Lexi::FontManager::Get().GetCharHeight() >
        m_params.y + m_params.height - m_botIndent;
}

std::shared_ptr<Row> Page::GetPreviousRow()
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
    return std::static_pointer_cast<Row>(*(std::prev(rowIt)));
}

std::shared_ptr<Row> Page::GetPreviousRow(IGlyph::GlyphPtr &row)
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), row);
    return std::static_pointer_cast<Row>(*(std::prev(rowIt)));
}

std::shared_ptr<Row> Page::GetNextRow()
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
    return std::static_pointer_cast<Row>(*std::next(rowIt));
}

std::shared_ptr<Row> Page::GetNextRow(IGlyph::GlyphPtr &row)
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), row);
    return std::static_pointer_cast<Row>(*std::next(rowIt));
}

// TODO(rmn): proper name
void Page::ProcessBackspace(Gui::Window* window)
{
    auto moveUpLowerRows = [&](auto startRowIt) {
        // Update the rows below
        for(; startRowIt != m_components.end(); ++startRowIt) {
            auto& row = *startRowIt;
            row->ClearGlyph(window);
            window->FillRectangle(row->GetGlyphParams(), Color::kWhite);  // TODO(rmn): fix
            row->MoveGlyph(0, -m_currentRow->GetHeight());
            row->Draw(window);
        }
    };

    if(m_currentRow == m_components.front()) {
        auto previousPage = m_parent->SwitchPage(window, TextView::SwitchDirection::kPrev, false);
        if(!previousPage) {
            m_currentRow->DrawCursor(window);
            return;
        }
        return;
    }

    auto prevRowIt = std::prev(std::find(m_components.begin(), m_components.end(), m_currentRow));
    auto previousRow = std::static_pointer_cast<Row>(*prevRowIt);

    if(m_currentRow->IsEmpty()) {
        m_components.remove(m_currentRow);
        m_currentRow = previousRow;
        m_currentRow->DrawCursorAtEnd(window);

        moveUpLowerRows(std::next(prevRowIt));

        // TODO(rmn): Update all other rows in other pages
        return;
    }

    // Replace last element of previous row with the first element of current
    if(previousRow->IsFull()) {
        auto removedElement = previousRow->GetLastChar();

        removedElement->SetChar(m_currentRow->GetFirstChar()->GetChar());
        removedElement->ReDraw(window);

        m_currentRow->Remove(window, m_currentRow->GetFirstChar());
        m_currentRow = previousRow;
        m_currentRow->DrawCursor(window);
    } else if(previousRow->IsEmpty()) {
        moveUpLowerRows(std::next(prevRowIt));
        m_components.erase(prevRowIt);
        m_currentRow->DrawCursor(window);
    } else {
        auto prevRowCapacity = previousRow->GetFreeSpace();
        if(prevRowCapacity >= m_currentRow->GetUsedSpace()) {
            previousRow->Insert(m_currentRow);
            if(m_currentRow == m_components.back()) {
                if(IsLastRow(m_currentRow)) {
                    // TODO(rmn): recursively remove lines from all pages
                } else {
                    m_currentRow->ClearGlyph(window);
                    window->FillRectangle(m_currentRow->GetGlyphParams(), Color::kWhite);

                    m_components.remove(m_currentRow);
                    m_currentRow = previousRow;
                    m_currentRow->DrawCursorAtEnd(window);
                }
            }
        } else {
            auto lst = m_currentRow->Cut(m_currentRow->GetPosition().x, prevRowCapacity);
            previousRow->Insert(previousRow->GetLastChar()->GetRightBorder(), std::move(lst));
            m_currentRow = previousRow;
            m_currentRow->DrawCursor(window);
        }
    }
}
