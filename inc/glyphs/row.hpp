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
    void ProcessEvent(Gui::Window* w, const Event& event) override {}

private:
    char m_symbol;
};

class Row: public ICompositeGlyph
{
public:
    explicit Row(TextView* parent, const GlyphParams& params);

    uint32_t GetLastCharPosition();
    void AddCharacter(Gui::Window* w, char c);

    void Draw(Gui::Window* window) override;

private:
    TextView* m_parent;

    uint32_t m_currentCharPosition = 0;
};

#endif  // LEXI_ROW_HPP
