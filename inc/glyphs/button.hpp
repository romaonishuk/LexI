//
// Created by romaonishuk on 14.12.19.
//

#ifndef LEXI_BUTTON_HPP
#define LEXI_BUTTON_HPP

#include <string>
#include "i_glyph.hpp"

class Button: public IGlyph
{
public:
    explicit Button(GlyphParams params, const std::string& text = "Button");
    void Draw(Gui::Window*) override;
    void Add(GlyphPtr) override {}

    void ProcessEvent(Gui::Window* w, const Event& event) override;

    [[nodiscard]] bool IsPressed() const { return m_isPressed; }

private:
    std::string m_text;
    bool m_isPressed = false;
};

#endif  // LEXI_BUTTON_HPP
