//
// Created by ronyshchuk on 19.04.20.
//

#ifndef LEXI_SCROLLER_HPP
#define LEXI_SCROLLER_HPP

#include "i_glyph.hpp"

#include <functional>

class Button;
class TextView;

class Scroller: public IGlyph
{
public:
    explicit Scroller(const GlyphParams& p, std::shared_ptr<TextView> view);

    void Draw(Gui::Window* window) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override;

    void UpdateScaling();
    void UpdateScrollerPosition(Gui::Window* w);

private:
    void SetButtonSize();

    std::shared_ptr<TextView> m_view;

    // How much TextView pixels are in one scrollBoard pixel
    float m_scale;
    std::shared_ptr<Button> m_scrollButton;
    Point m_Previous;  // TODO(rmn): rename!!!
};

#endif  // LEXI_SCROLLER_HPP