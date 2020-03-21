//
// Created by romaonishuk on 14.12.19.
//

#ifndef LEXI_BUTTON_HPP
#define LEXI_BUTTON_HPP

#include "i_glyph.hpp"
#include <string>

class Button : public IGlyph
{
public:
    explicit Button(GlyphParams params, const std::string& text="Button");
    void Draw(Gui::Window *) override;
    void Add(GlyphPtr) override {}

    void OnButtonPressed(Gui::Window *);
    void OnButtonReleased(Gui::Window *);

    void ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev) override;
public:
    std::string m_text;
};

#endif //LEXI_BUTTON_HPP
