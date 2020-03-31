//
// Created by romaonishuk on 14.12.19.
//

#ifndef LEXI_TEXT_VIEW_HPP
#define LEXI_TEXT_VIEW_HPP

#include "i_glyph.hpp"

class TextView: public ICompositeGlyph
{
public:
    explicit TextView(const GlyphParams& params): ICompositeGlyph(params) {}

    void Draw(Gui::Window* window) override
    {
        window->FillRectangle({m_params.x, m_params.y}, m_params.width, m_params.height, Color::kWhite);
        ICompositeGlyph::Draw(window);
    }
};

#endif  // LEXI_TEXT_VIEW_HPP
