//
// Created by romaonishuk on 14.12.19.
//
#ifndef LEXI_TEXT_LABEL_HPP
#define LEXI_TEXT_LABEL_HPP

#include "glyphs.hpp"

#include <cassert>

#include "logger.hpp"
#include "types.hpp"
#include "window.hpp"

class TextLabel : public IGlyph
{
public:
    explicit TextLabel(GlyphParams params, std::string text = "TextLabel") :
    IGlyph(params), m_text(text)
    {
    }

    void Draw(Gui::Window *) override;
    void ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev) override{}
private:
    std::string m_text;
};


// ---- Text Label ----

void TextLabel::Draw(Gui::Window *w)
{
    w->DrawText({m_params.x, m_params.y}, m_text);
}

#endif //LEXI_TEXT_LABEL_HPP
