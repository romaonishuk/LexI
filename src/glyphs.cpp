//
// Created by romaonishuk on 28.10.19.
//

#include "glyphs.hpp"

#include <cassert>

#include "logger.hpp"
#include "types.hpp"
#include "window.hpp"

// --------------------------------------------

// // TODO(rmn): Move this!
// // TODO(rmn): other types

class TextLabel : public IGlyph
{
public:
    explicit TextLabel(GlyphParams params, std::string="TextLabel");
    void Draw(Gui::Window *) override;
private:
    std::string m_text;
};


// ---- Text Label ----
TextLabel::TextLabel(GlyphParams params, std::string text) : IGlyph(params)
{
    m_text = text;
}
void TextLabel::Draw(Gui::Window *w)
{
    w->DrawText({m_params.x, m_params.y}, m_text);
}

///// ---- Character ---
//void Character::Draw(Gui::Window *w)
//{
//    assert(w);
//    w->DrawRectangle({33, 22}, 123, 124);
//}
//
//
///// --- Row ---
//void Row::Draw(Gui::Window *w)
//{
//    assert(w);
//    w->DrawRectangle({33, 22}, 400, 500);
//}
