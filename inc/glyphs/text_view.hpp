//
// Created by romaonishuk on 14.12.19.
//

#ifndef LEXI_TEXT_VIEW_HPP
#define LEXI_TEXT_VIEW_HPP

#include "window.hpp"

class Page;

class TextView: public Gui::Window
{
public:
    explicit TextView(const GlyphParams&, Window*);

    void Draw(Gui::Window* window) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override;

    void DrawRectangle(const GlyphParams& params) const override;
    void DrawText(const Point& text_position, std::string text) const override;
    void DrawLine(const Point& start_point, const Point& end_point) const override;

    void UpdateVisibleArea(height_t h);
    void MoveVisibleArea(height_t h);

    [[nodiscard]] std::string GetWindowName() const override { return "Text View"; }

private:
    // Relative square currently shown on display.
    GlyphParams m_visibleArea;

    std::shared_ptr<Page> m_currentPage;
    std::list<std::shared_ptr<Page>> m_visiblePages;
};

#endif  // LEXI_TEXT_VIEW_HPP
