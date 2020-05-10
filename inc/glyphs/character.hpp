//
// Created by ronyshchuk on 09.05.20.
//

#ifndef LEXI_CHARACTER_HPP
#define LEXI_CHARACTER_HPP

#include "i_glyph.hpp"

// TODO(rmn): optimize
class Character: public IGlyph
{
public:
    Character(const GlyphParams& params, char c): IGlyph(params), m_symbol(c) {}
    void Draw(Gui::Window* window) override;
    void DrawAt(Gui::Window*, const Point& point) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override {}

    void SetChar(char c) { m_symbol = c; }
    [[nodiscard]] char GetChar() const { return m_symbol; }

private:
    char m_symbol;
};

#endif  // LEXI_CHARACTER_HPP
