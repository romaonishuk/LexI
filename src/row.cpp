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

Row::Row(const GlyphParams& params, ICompositeGlyph::GlyphList&& list):
    ICompositeGlyph(params){
    Insert(0, std::move(list));
}

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
    cursor.MoveCursor(window, {pos, m_params.y, cursorWidth, m_params.height});
}

void Row::ProcessEvent(Gui::Window* window, const Event& event)
{
    if(event.GetEvent() != Lexi::EventType::KeyPressed) {
        return;
    }

    const auto* key = dynamic_cast<const Lexi::KeyBoardEvent*>(&event);
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

IGlyph::GlyphPtr Row::InsertChar(Gui::Window* window, std::shared_ptr<Character>& newChar, const Point& position)
{
    const auto& charWidth = newChar->GetWidth();

    if(m_components.empty()) {
        Add(newChar);
        newChar->Draw(window);
        m_usedWidth += (charWidth + 1);
        return nullptr;
    }

    auto& lastChar = m_components.back();
    // Add to end. No need to redraw other characters
    if(lastChar->Intersects(position) && position.x != m_params.x) {
        if(IsFull()) {
            return newChar;
        } else {
            newChar->SetPosition(lastChar->GetRightBorder() + 1, m_params.y);
            Add(newChar);
            newChar->Draw(window);
        }
    } else {
        if(position.x == m_params.x) {
            m_components.push_front(newChar);
            UpdateRestElements(++m_components.begin(), static_cast<int>(charWidth + 1));
        } else {
            auto it = std::find_if(m_components.begin(), m_components.end(),
                [&](const auto& element) { return element->Intersects(position); });

            assert(it != m_components.end());

            newChar->SetPosition((*it)->GetRightBorder() + 1, m_params.y);
            m_components.insert(++it, newChar);
            UpdateRestElements(it, static_cast<int>(charWidth + 1));
        }

        m_usedWidth += (charWidth + 1);
        auto res = RemoveRedundantCharacter();
        ReDraw(window);
        return res;
    }

    m_usedWidth += (charWidth + 1);
    return nullptr;
}

std::optional<IGlyph::GlyphPtr> Row::AddCharacter(char c)
{
    const auto& fontManager = Lexi::FontManager::Get();
    const auto charWidth = fontManager.GetCharWidth();
    const auto charHeight = fontManager.GetCharHeight();
    auto* window = cursor.GetCurrentWindow();

    // TODO(rmn): move
    auto newChar = std::make_shared<Character>(GlyphParams{m_params.x, m_params.y, charWidth, charHeight}, c);
    auto poppedElement = InsertChar(window, newChar, cursor.GetPosition());

    cursor.MoveCursor(window, {newChar->GetRightBorder(), m_params.y, cursorWidth, m_params.height});
    return poppedElement ? std::optional<GlyphPtr>(std::move(poppedElement)) : std::nullopt;
}

std::optional<IGlyph::GlyphPtr> Row::AddCharacter(const Point& position, IGlyph::GlyphPtr newChar)
{
    if(!Intersects(position)) {
        return std::nullopt;
    }

    auto* window = cursor.GetCurrentWindow();
    newChar->SetPosition(position);
    auto c = std::static_pointer_cast<Character>(newChar);
    auto poppedElement = InsertChar(window, c, position);
    return poppedElement ? std::optional<GlyphPtr>(std::move(poppedElement)) : std::nullopt;
}

std::optional<ICompositeGlyph::GlyphList> Row::Insert(std::shared_ptr<Row>& row)
{
    auto position = m_components.empty() ? m_params.x : m_components.back()->GetRightBorder();
    auto tmpList = row->m_components;
    return Insert(position, std::move(tmpList));
}

std::optional<ICompositeGlyph::GlyphList> Row::Insert(std::shared_ptr<Row>&& row)
{
    if(row->m_components.empty()) {
        return {};
    }

    auto position = m_components.empty() ? m_params.x : m_components.back()->GetRightBorder();
    if(m_components.empty()) {
        AddCharacter({m_params.x, m_params.y}, row->m_components.front());
        row->m_components.pop_front();
    }
    return Insert(position, std::move(std::move(row->m_components)));
}

std::optional<ICompositeGlyph::GlyphList> Row::Insert(size_t insertPosition, std::list<GlyphPtr>&& itemsToInsert)
{
    auto updateElements = [&](width_t offset){
      for(auto& it: itemsToInsert) {
          it->SetPosition(offset, m_params.y);
          offset += it->GetWidth() + 1;
      }
    };

    std::optional<GlyphList> result;

    width_t itemsToInsertWidth =
        std::accumulate(itemsToInsert.begin(), itemsToInsert.end(), 0, [](auto& init, const auto& item) {
            init += item->GetWidth();
            return init;
        });

    if(m_components.empty()) {
        updateElements(m_params.x);
        std::swap(m_components, itemsToInsert);
        m_usedWidth += itemsToInsertWidth;
        ReDraw(cursor.GetCurrentWindow());
        return result;
    }

    auto insertionElement = std::find_if(m_components.begin(), m_components.end(),
        [&](const auto& glyph) { return glyph->Intersects(Point(insertPosition, m_params.y)); });
    assert(insertionElement != m_components.end());
    // Insert After
    if((*insertionElement)->GetRightBorder() == insertPosition) {
        insertionElement++;
    }

    width_t offset = (*std::prev(insertionElement))->GetRightBorder() + 1;
    updateElements(offset);

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

void Row::ReWrite(std::shared_ptr<Row>&& row)
{
    m_components.clear();
    m_usedWidth = 0;

    m_components = std::move(row->m_components);
    for(auto& it: m_components) {
        it->SetPosition(it->GetPosition().x, m_params.y);
    }
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

IGlyph::GlyphPtr Row::RemoveRedundantCharacter()
{
    if(m_usedWidth > m_params.width) {
        auto tmp = m_components.back();
        m_usedWidth -= tmp->GetWidth() - 1;
        m_components.pop_back();
        return tmp;
    }
    return nullptr;
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
        result.splice(result.begin(), m_components, firstCutElement, m_components.end());
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
