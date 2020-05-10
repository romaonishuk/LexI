//
// Created by romaonishuk on 28.10.19.
//

#include "i_glyph.hpp"

#include "types.hpp"
#include "window.hpp"

void IGlyph::ClearGlyph(Gui::Window* w)
{
    w->ClearArea(m_params);
}
