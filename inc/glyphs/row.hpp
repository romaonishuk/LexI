//
// Created by ronyshchuk on 08.04.20.
//

#ifndef LEXI_ROW_HPP
#define LEXI_ROW_HPP

#include "i_composite_glyph.hpp"

class TextView;

// TODO(rmn): optimize
class Character: public IGlyph
{
public:
    Character(const GlyphParams& params, char c): IGlyph(params), m_symbol(c) {}
    void Draw(Gui::Window* window) override;
    void DrawAt(Gui::Window*, const Point& point) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override {}

private:
    char m_symbol;
};

class Row: public ICompositeGlyph
{
public:
    explicit Row(const GlyphParams& params);

    void AddCharacter(Gui::Window* w, char c);

    void Draw(Gui::Window* window) override;
    void DrawCursor(Gui::Window* window);
    //    void DrawCursorLeft(Gui::Window* window);
    //    void DrawCursorRight(Gui::Window* window);

    bool IsEmpty() const { return m_components.empty(); }
    bool IsFull() const;

    GlyphPtr GetLast() const;

    void ProcessEvent(Gui::Window* w, const Event& event) override;
    void Remove(Gui::Window* w, const GlyphPtr& ptr);

private:
    width_t m_usedWidth = 0;
};

#endif  // LEXI_ROW_HPP
