//
// Created by ronyshchuk on 23.04.20.
//

#ifndef LEXI_GRAPHIC_PRIMITIVES_HPP
#define LEXI_GRAPHIC_PRIMITIVES_HPP

#include "window.hpp"
namespace {
void DrawButton(Gui::Window* w, const GlyphParams& params)
{
    w->SetForeground(Color::kWhite);

    // top
    w->DrawLine({params.x, params.y}, {params.x + params.width, params.y});
    // left
    w->DrawLine({params.x, params.y}, {params.x, params.y + params.height});

    w->SetForeground(Color::kBlack);

    // bottom
    w->DrawLine({params.x, params.y + params.height}, {params.x + params.width, params.y + params.height});

    // right
    w->DrawLine({params.x + params.width, params.y}, {params.x + params.width, params.y + params.height});
}

void DrawPressedButton(Gui::Window* w, const GlyphParams& params)
{
    w->SetForeground(Color::kWhite);

    // bottom
    w->DrawLine({params.x, params.y + params.height}, {params.x + params.width, params.y + params.height});
    // right
    w->DrawLine({params.x + params.width, params.y}, {params.x + params.width, params.y + params.height});

    w->SetForeground(Color::kBlack);

    // top
    w->DrawLine({params.x, params.y}, {params.x + params.width, params.y});
    // left
    w->DrawLine({params.x, params.y}, {params.x, params.y + params.height});

    w->SetForeground(Color::kGray);
}
}  // namespace

#endif  // LEXI_GRAPHIC_PRIMITIVES_HPP
