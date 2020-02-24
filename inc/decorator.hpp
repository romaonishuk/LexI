//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_DECORATOR_HPP
#define LEXI_DECORATOR_HPP

#include "glyphs.hpp"
#include "window.hpp"


class IDecorator : public IGlyph
{
public:
    IDecorator(const GlyphParams params, GlyphPtr decorable): 
                IGlyph(params), m_decorable(std::move(decorable)){}
    virtual ~IDecorator() = default;
    // virtual void Draw() = 0;

    void ProcessEvent(Gui::Window *w,const Point& p, const EventType& ev) = 0;

    GlyphPtr m_decorable;
};

class BorderDecorator : public IDecorator
{
public:
    BorderDecorator(const GlyphParams params, GlyphPtr decorable): IDecorator(params, std::move(decorable)){}
    void Draw(Gui::Window* w) override {
        DrawBorder(w);
    }

    void ProcessEvent(Gui::Window *w,const Point& p, const EventType& ev) {
        // TODO(rmn):
        m_decorable->ProcessEvent(w, p, ev);
    }

    void DrawBorder(Gui::Window* w) {
        m_decorable->Draw(w);
        w->SetForeground(Color::kBlack);
        w->DrawRectangle({m_params.x, m_params.y}, m_params.width, m_params.height);
    }
};

// class ScrollDecorator : public Decorator
// {
// public:
//     void ScrollTo();
//     void Draw() override;
// };

#endif //LEXI_DECORATOR_HPP
