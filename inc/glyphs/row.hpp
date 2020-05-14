//
// Created by ronyshchuk on 08.04.20.
//

#ifndef LEXI_ROW_HPP
#define LEXI_ROW_HPP

#include "i_composite_glyph.hpp"

#include <optional>

class TextView;
class Character;

class Row: public ICompositeGlyph
{
public:
    explicit Row(const GlyphParams& params);

    void ProcessEvent(Gui::Window* w, const Event& event) override;
    void Draw(Gui::Window* window) override;
    void DrawCursor(Gui::Window* window);
    void DrawCursorAtEnd(Gui::Window* window);
    void ReDraw(Gui::Window* window) override;

    std::optional<GlyphPtr> AddCharacter(char c);
    std::optional<GlyphPtr> AddCharacter(const Point& position, IGlyph::GlyphPtr newChar);
    GlyphPtr InsertChar(Gui::Window* window, std::shared_ptr<Character> &newChar, const Point& p);

    void Remove(Gui::Window* window, const GlyphPtr& ptr);
    GlyphList Cut(size_t startPosition, size_t pixelsCount);

    using ICompositeGlyph::Insert;
    std::optional<GlyphList> Insert(std::shared_ptr<Row>& row);
    std::optional<GlyphList> Insert(size_t insertPosition, std::list<GlyphPtr>&&);

    [[nodiscard]] bool IsEmpty() const { return m_components.empty(); }
    [[nodiscard]] bool IsFull() const;
    [[nodiscard]] width_t GetFreeSpace() const { return m_params.width - m_usedWidth; }
    [[nodiscard]] width_t GetUsedSpace() const { return m_usedWidth; }
    [[nodiscard]] std::shared_ptr<Character> GetFirstChar() const;
    [[nodiscard]] std::shared_ptr<Character> GetLastChar() const;

private:
    void Remove(Gui::Window* window, GlyphList::iterator& it);
    IGlyph::GlyphPtr RemoveRedundantCharacter();
    void UpdateRestElements(GlyphList::iterator& it, const int updateValue);

    width_t m_usedWidth = 0;
};

#endif  // LEXI_ROW_HPP
