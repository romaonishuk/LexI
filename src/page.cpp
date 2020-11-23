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

using Lexi::FontManager;

Page::Page(TextView* parent, const GlyphParams& params): ICompositeGlyph(params), m_parent(parent)
{
    m_currentRow = std::make_shared<Row>(
        GlyphParams{m_leftIndent, m_params.y + m_topIndent + FontManager::Get().GetCharHeight(),
            m_params.width - m_leftIndent - m_rightIndent, FontManager::Get().GetCharHeight()});
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
        if(IsBottomRow(row)) {
            nextPage = m_parent->GetNextPage(this);
            if(!nextPage) {
                nextPage = m_parent->AddPage(Lexi::Cursor::Get().GetCurrentWindow(), this);
            }
            nextRow = nextPage->GetFirstRow();
            nextPage->SetCurrentRow(nextRow);
        } else {
            nextRow = std::make_shared<Row>(
                GlyphParams{m_leftIndent, row->GetPosition().y + FontManager::Get().GetCharHeight(),
                    m_params.width - m_leftIndent - m_rightIndent, FontManager::Get().GetCharHeight()});
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
            auto movedItems = m_currentRow->Cut(cursor.GetPosition().x + 1, m_currentRow->GetWidth());
            GlyphParams params{m_currentRow->GetGlyphParams()};
            params.y += m_currentRow->GetHeight();

            auto newRow = std::make_shared<Row>(params, std::move(movedItems));
            Insert(newRow, GetGlyphPosition(std::static_pointer_cast<IGlyph>(m_currentRow)) + 1);

            auto isLast = IsBottomRow(m_currentRow);
            m_currentRow = newRow;
            cursor.MoveCursor(window, m_currentRow->GetGlyphParams());

            // Recursively update all pages below
            auto currentPage = this;
            while(currentPage->m_components.back()->GetBottomBorder() + m_botIndent >= currentPage->GetBottomBorder()) {
                auto nextPage = currentPage->m_parent->GetNextPage(currentPage);
                if(!nextPage) {
                    std::cout << "Creating new page as a result of Enter press" << std::endl;
                    nextPage = currentPage->m_parent->AddPage(window, currentPage);
                }

                auto lastRow = currentPage->m_components.back();
                currentPage->m_components.pop_back();

                auto deltaY = nextPage->GetGlyphParams().y + m_topIndent +
                        FontManager::Get().GetCharHeight() - lastRow->GetPosition().y;
                lastRow->MoveGlyph(0, deltaY);
                nextPage->Insert(lastRow, 0);
                currentPage = nextPage.get();
            }

            if(isLast) {
                m_parent->SwitchPage(window, TextView::SwitchDirection::kNext, false);
            }

            // TODO(rmn): Redrawing whole page not optimal way
            m_parent->UpdateVisiblePages();
            m_parent->ReDraw(window);
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
                ProcessBackspaceFromBeginning(window);
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

bool Page::IsBottomRow(const GlyphPtr& row) const
{
    return row->GetBottomBorder() + Lexi::FontManager::Get().GetCharHeight() >
        m_params.y + m_params.height - m_botIndent;
}

std::shared_ptr<Row> Page::GetPreviousRow()
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
    return std::static_pointer_cast<Row>(*(std::prev(rowIt)));
}

std::shared_ptr<Row> Page::GetPreviousRow(IGlyph::GlyphPtr& row)
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), row);
    return std::static_pointer_cast<Row>(*(std::prev(rowIt)));
}

std::shared_ptr<Row> Page::GetNextRow()
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), m_currentRow);
    return std::static_pointer_cast<Row>(*std::next(rowIt));
}

std::shared_ptr<Row> Page::GetNextRow(IGlyph::GlyphPtr& row)
{
    auto rowIt = std::find(m_components.begin(), m_components.end(), row);
    return std::static_pointer_cast<Row>(*std::next(rowIt));
}

void Page::MoveUpLowerRows(Gui::Window* window, std::list<IGlyph::GlyphPtr>::iterator startRowIt)
{
    // Update the rows below
    for(; startRowIt != m_components.end(); ++startRowIt) {
        auto& row = *startRowIt;
        row->ClearGlyph(window);
        window->FillRectangle(row->GetGlyphParams(), Color::kWhite);  // TODO(rmn): fix
        row->MoveGlyph(0, -m_currentRow->GetHeight());
        row->Draw(window);
    }
}

std::shared_ptr<Row> Page::RemoveFirstRow()
{
    auto result = std::static_pointer_cast<Row>(m_components.front());
    m_currentRow = result;

    auto* window = Lexi::Cursor::Get().GetCurrentWindow();
    m_currentRow->ClearGlyph(window);
    window->FillRectangle(m_currentRow->GetGlyphParams(), Color::kWhite);

    auto isFull = IsFull();
    m_components.pop_front();

    MoveUpLowerRows(Lexi::Cursor::Get().GetCurrentWindow(), m_components.begin());

    if(isFull) {
        auto nextPage = m_parent->GetNextPage(this);
        auto newLastRow = nextPage->RemoveFirstRow();
        newLastRow->SetPosition(newLastRow->GetPosition().x, m_components.back()->GetBottomBorder());
        m_components.push_back(newLastRow);
    }

    return std::static_pointer_cast<Row>(result);
}

void Page::ProcessBackspaceFromBeginning(Gui::Window* window)
{
    if(m_currentRow == m_components.front()) {
        auto previousPage = m_parent->SwitchPage(window, TextView::SwitchDirection::kPrev, false);
        //        if(previousPage) {
        //            if(previousPage->IsFull()) {
        //                m_parent->RemovePage(previousPage);
        //                m_parent->SetCurrentPage(this);
        //            }
        //        }

        m_currentRow->DrawCursor(window);
        return;
    }

    auto prevRowIt = std::prev(std::find(m_components.begin(), m_components.end(), m_currentRow));
    auto previousRow = std::static_pointer_cast<Row>(*prevRowIt);

    if(m_currentRow->IsEmpty()) {
        m_components.remove(m_currentRow);
        m_currentRow = previousRow;
        m_currentRow->DrawCursorAtEnd(window);

        MoveUpLowerRows(window, std::next(prevRowIt));
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
        MoveUpLowerRows(window, std::next(prevRowIt));
        m_components.erase(prevRowIt);
        m_currentRow->DrawCursor(window);
    } else {
        auto prevRowCapacity = previousRow->GetFreeSpace();
        if(prevRowCapacity >= m_currentRow->GetUsedSpace()) {
            m_currentRow->ClearGlyph(window);
            window->FillRectangle(m_currentRow->GetGlyphParams(), Color::kWhite);
            previousRow->Insert(m_currentRow);
            // Removed last line in a page.
            if(m_currentRow == m_components.back()) {
                // Need to update all other pages
                if(IsBottomRow(m_currentRow)) {
                    auto nextPage = m_parent->GetNextPage(this);
                    if(!nextPage->IsEmpty()) {
                        m_currentRow->ReWrite(nextPage->RemoveFirstRow());
                    }

                    if(nextPage->IsEmpty()) {
                        m_parent->RemovePage(nextPage);
                    }
                } else {
                    m_components.remove(m_currentRow);
                }

                m_currentRow->Draw(window);
                m_currentRow = previousRow;
                m_currentRow->DrawCursorAtEnd(window);
            }
        } else {
            auto lst = m_currentRow->Cut(m_currentRow->GetPosition().x, prevRowCapacity);
            previousRow->Insert(previousRow->GetLastChar()->GetRightBorder(), std::move(lst));
            m_currentRow = previousRow;
            m_currentRow->DrawCursor(window);
        }
    }
}

void Page::Insert(GlyphPtr glyph, size_t position)
{
    ICompositeGlyph::Insert(glyph, position);

    auto it = std::find(m_components.begin(), m_components.end(), glyph);
    assert((it != m_components.end()) && "Couldn't find previously inserted glyph");

    for(it = std::next(it); it != m_components.end(); ++it) {
        auto& row = *it;
        row->MoveGlyph(0, glyph->GetHeight());
    }
}

bool Page::IsFull() const
{
    return m_components.back()->GetPosition().y + Lexi::FontManager::Get().GetCharWidth() >= GetBottomBorder();
}
