//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_DECORATOR_HPP
#define LEXI_DECORATOR_HPP

#include "i_glyph.hpp"
#include "window.hpp"

// TODO(rmn):
// - use array + templates to remove

class IDecorator: public ICompositeGlyph
{
public:
    explicit IDecorator(const GlyphParams& params): ICompositeGlyph(params) {}
    ~IDecorator() override = default;
    // virtual void Draw() = 0;
};

class BorderDecorator: public IDecorator
{
public:
    BorderDecorator(const GlyphParams params, Color borderColor): IDecorator(params), m_color(borderColor) {}

    void Draw(Gui::Window* w) override { DrawBorder(w); }

    void DrawBorder(Gui::Window* w)
    {
        for(const auto& it: m_components) {
            it->Draw(w);
        }

        w->SetForeground(m_color);
        w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
    }

private:
    Color m_color;
};

// class ScrollDecorator : public Decorator
// {
// public:
//     void ScrollTo();
//     void Draw() override;
// };

#endif  // LEXI_DECORATOR_HPP
