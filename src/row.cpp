//
// Created by ronyshchuk on 08.04.20.
//

#include "row.hpp"

#include <algorithm>
#include <numeric>

#include "character.hpp"
#include "cursor.hpp"
#include "font.hpp"
#include "window.hpp"

constexpr auto cursorWidth = 1;
auto& cursor = Lexi::Cursor::Get();

Row::Row(const GlyphParams& params): ICompositeGlyph(params)
{}

bool Row::IsFull() const
{
    return m_usedWidth + Lexi::FontManager::Get().GetCharWidth() >= m_params.width;
}

void Row::Draw(Gui::Window* window)
{
    window->SetForeground(Color::kBlack);
    ICompositeGlyph::Draw(window);
}

// TODO(rmn): This should be fixed in text_view
void Row::ReDraw(Gui::Window* window)
{
    ClearGlyph(window);
    window->FillRectangle(m_params, Color::kWhite);
    Draw(window);
}

void Row::DrawCursor(Gui::Window* window)
{
    auto cursorPosition = cursor.GetPosition();
    cursorPosition.y = m_params.y;
    if(m_components.empty() || cursorPosition.x == m_params.x) {
        cursor.MoveCursor(window, {m_params.x, m_params.y, cursorWidth, m_params.height});
        return;
    }

    for(auto& it: m_components) {
        if(it->Intersects(cursorPosition)) {
            cursor.MoveCursor(window, {it->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
            return;
        }
    }

    auto& lastElement = m_components.back();
    cursor.MoveCursor(window, {lastElement->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
}

void Row::DrawCursorAtEnd(Gui::Window* window)
{
    auto& lastChar = m_components.back();
    auto pos = m_params.x;
    if(lastChar) {
        pos = lastChar->GetRightBorder();
    }
    Lexi::Cursor::Get().MoveCursor(window, {pos, m_params.y, cursorWidth, m_params.height});
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
            cursor.MoveCursor(window, {prevGlyph->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
        }
    } else if(key->GetKey() == Lexi::Key::kArrowRight) {
        if(cursorPosition.x == m_params.x) {
            const auto& params = (*currentGlyph)->GetGlyphParams();
            cursor.MoveCursor(window, {params.x + params.width, m_params.y, cursorWidth, m_params.height});
        } else if(currentGlyph != --m_components.end()) {
            const auto& nextGlyph = *(std::next(currentGlyph));
            const auto& params = nextGlyph->GetGlyphParams();
            cursor.MoveCursor(window, {params.x + params.width, m_params.y, cursorWidth, m_params.height});
        }
    } else if(key->GetKey() == Lexi::Key::kBackspace) {
        Remove(window, currentGlyph);
    }
}

std::shared_ptr<Character> Row::GetFirstChar() const
{
    return m_components.empty() ? nullptr : std::static_pointer_cast<Character>(m_components.front());
}

std::shared_ptr<Character> Row::GetLastChar() const
{
    return m_components.empty() ? nullptr : std::static_pointer_cast<Character>(m_components.back());
}

// --- Insertions ---

void Row::UpdateRestElements(GlyphList::iterator& it, const int updateValue)
{
    for(; it != m_components.end(); ++it) {
        const auto& currentPos = (*it)->GetPosition();
        (*it)->SetPosition(currentPos.x + updateValue, currentPos.y);
    }
}

std::optional<std::shared_ptr<IGlyph>> Row::AddCharacter(Gui::Window* w, char c)
{
    const auto& fontManager = Lexi::FontManager::Get();
    const auto charWidth = fontManager.GetCharWidth();
    const auto charHeight = fontManager.GetCharHeight();

    // TODO(rmn): move
    auto newChar = std::make_shared<Character>(GlyphParams{m_params.x, m_params.y, charWidth, charHeight}, c);
    if(m_components.empty()) {
        Add(newChar);
        newChar->Draw(w);
        cursor.MoveCursor(w, {newChar->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
        m_usedWidth += (charWidth + 1);
        return std::nullopt;
    }

    const auto& cursorPosition = cursor.GetPosition();
    auto& lastChar = m_components.back();
    // Add to end. No need to redraw other characters
    if(lastChar->Intersects(cursorPosition)) {
        newChar->SetPosition(lastChar->GetRightBorder() + 1, m_params.y);
        Add(newChar);
        newChar->Draw(w);
    } else {
        if(cursorPosition.x == m_params.x) {
            m_components.push_front(newChar);
            UpdateRestElements(++m_components.begin(), static_cast<int>(charWidth + 1));
        } else {
            auto it = std::find_if(m_components.begin(), m_components.end(),
                [&](const auto& element) { return element->Intersects(cursorPosition); });

            assert(it != m_components.end());

            newChar->SetPosition((*it)->GetRightBorder() + 1, m_params.y);
            m_components.insert(++it, newChar);
            UpdateRestElements(it, static_cast<int>(charWidth + 1));
        }
        ReDraw(w);
    }

    cursor.MoveCursor(w, {newChar->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
    m_usedWidth += (charWidth + 1);

    if(m_usedWidth > m_params.width) {
        auto tmp = m_components.back();
        m_usedWidth -= tmp->GetWidth() - 1;
        m_components.pop_back();
        return std::optional<std::shared_ptr<IGlyph>>(std::move(tmp));
    }
    return std::nullopt;
}

std::optional<ICompositeGlyph::GlyphList> Row::Insert(std::shared_ptr<Row>& row)
{
    auto position = m_components.back()->GetRightBorder();
    auto tmpList = row->m_components;
    return Insert(position, std::move(tmpList));
}

std::optional<ICompositeGlyph::GlyphList> Row::Insert(
    size_t insertPosition, std::list<IGlyph::GlyphPtr>&& itemsToInsert)
{
    std::optional<GlyphList> result;

    width_t itemsToInsertWidth =
        std::accumulate(itemsToInsert.begin(), itemsToInsert.end(), 0, [](auto& init, const auto& item) {
            init += item->GetWidth();
            return init;
        });

    auto insertionElement = std::find_if(m_components.begin(), m_components.end(),
        [&](const auto& glyph) { return glyph->Intersects(Point(insertPosition, m_params.y)); });
    assert(insertionElement != m_components.end());
    // Insert After
    if((*insertionElement)->GetRightBorder() == insertPosition) {
        insertionElement++;
    }

    width_t offset = (*std::prev(insertionElement))->GetRightBorder() + 1;
    for(auto& it: itemsToInsert) {
        it->SetPosition(offset, m_params.y);
        offset += it->GetWidth() + 1;
    }

    m_components.insert(insertionElement, itemsToInsert.begin(), itemsToInsert.end());

    // update rest of elements
    UpdateRestElements(insertionElement, itemsToInsertWidth);

    m_usedWidth += itemsToInsertWidth;

    if(m_usedWidth > m_params.width) {
        auto lastElement = std::find_if(m_components.begin(), m_components.end(),
            [&](const auto& element) { return element->GetRightBorder() > GetRightBorder(); });

        assert((lastElement != m_components.end()));

        ICompositeGlyph::GlyphList tempo;
        tempo.splice(tempo.begin(), m_components, lastElement);
        result.emplace(std::move(tempo));
    }

    ReDraw(cursor.GetCurrentWindow());
    //    cursor.MoveCursor(window, {position, m_params.y, cursorWidth, m_params.height});
    return result;
}

// --- Removal ---

void Row::Remove(Gui::Window* window, const GlyphPtr& ptr)
{
    assert(ptr);

    auto it = std::find(m_components.begin(), m_components.end(), ptr);
    if(it != m_components.end()) {
        Remove(window, it);
    }
}

void Row::Remove(Gui::Window* window, ICompositeGlyph::GlyphList::iterator& it)
{
    auto charWidth = (*it)->GetWidth();
    it = m_components.erase(it);
    for(; it != m_components.end(); ++it) {
        auto& nextChar = (*it);
        nextChar->SetPosition({nextChar->GetPosition().x - charWidth - 1, m_params.y});
    }

    ReDraw(window);

    // NOTE: need to consider fact that we add 1 px to distinguish characters
    auto newCursorPosition = cursor.GetPosition().x - charWidth - 1;
    if(newCursorPosition < m_params.x) {
        newCursorPosition = m_params.x;
    }

    cursor.MoveCursor(window, {newCursorPosition, m_params.y, cursorWidth, m_params.height});
    m_usedWidth -= charWidth;
}

ICompositeGlyph::GlyphList Row::Cut(size_t startPosition, size_t pixelsCount)
{
    std::list<IGlyph::GlyphPtr> result;
    if(pixelsCount == 0) {
        return result;
    }

    auto firstCutElement = std::find_if(m_components.begin(), m_components.end(),
        [&](const auto& glyph) { return glyph->Intersects(Point(startPosition, m_params.y)); });
    if(firstCutElement == m_components.end()) {
        std::cout << "Failed to cut glyph at starting position: " << startPosition << std::endl;
        return result;
    }

    auto lastPixel = startPosition + pixelsCount;
    auto lastCutElement = std::find_if(firstCutElement, m_components.end(),
        [&](const auto& glyph) { return glyph->Intersects(Point(lastPixel, m_params.y)); });
    // get all chars that left
    // TODO(rmn): what about images?
    if(lastCutElement == m_components.end()) {
        result.splice(result.begin(), m_components, firstCutElement);
    } else {
        // We don't need half of symbol
        if(lastPixel == (*lastCutElement)->GetRightBorder()) {
            lastCutElement = std::next(lastCutElement);
        }

        // NOTE: splice range is [firstCutElement, last)
        result.splice(result.begin(), m_components, firstCutElement, lastCutElement);
    }

    auto cutWidth = std::accumulate(result.begin(), result.end(), 0, [](auto& init, const auto& item) {
        init += item->GetWidth() + 1;
        return init;
    });

    m_usedWidth -= cutWidth;

    for(; lastCutElement != m_components.end(); ++lastCutElement) {
        const auto& currentPos = (*lastCutElement)->GetPosition();
        (*lastCutElement)->SetPosition(currentPos.x - cutWidth, currentPos.y);
    }

    auto* window = cursor.GetCurrentWindow();
    ReDraw(window);
    //    cursor.MoveCursor(window, {position, m_params.y, cursorWidth, m_params.height});
    return result;
}
