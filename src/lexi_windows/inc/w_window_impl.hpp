//
// Created by romaonishuk on 01.12.2020.
//

#ifndef LEXI_W_WINDOW_IMPL_HPP
#define LEXI_W_WINDOW_IMPL_HPP

#include "window_impl.hpp"
#include "font.hpp"

namespace Gui {
class WWindowImpl : public WindowImpl {
public:
    explicit WWindowImpl(const GlyphParams &);

    ~WWindowImpl() override{}

    void DrawRectangle(const Point &point, const width_t width, const height_t height) override{}

    void DrawText(const Point &text_position, std::string text) override{}

    void DrawText(const GlyphParams &params, const std::string &text, Alignment alignment) override{}

    void DrawLine(const Point &start_point, const Point &end_point) override{}

    void SetForeground(const int color) override{}

    void FillRectangle(const Point &point, const width_t width, const height_t height,
                       const Color color) override{}

    void ShowWindow() override{}

    void HideWindow() override{}

    void ClearWindow() override{}

    void ClearGlyph(const GlyphParams &p, bool sendExposureEvent) override{}

    void Resize(width_t width, height_t height) override{}

    void SetFontPath(const std::string &path) override{}

    std::set<Lexi::FontName> GetFontList() override{return {Lexi::FontName{"", ""}};}

    bool ChangeFont(Lexi::Font &) override{return false;}

    unsigned long GetWindow() const override {return 0;};

    void* GetDisplay() const override {return nullptr;};

};
}

#endif //LEXI_W_WINDOW_IMPL_HPP
