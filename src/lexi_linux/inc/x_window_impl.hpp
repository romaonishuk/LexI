//
// Created by romaonishuk on 01.12.19.
//

#ifndef LEXI_X_WINDOW_IMPL_CPP_HPP
#define LEXI_X_WINDOW_IMPL_CPP_HPP

#include "font.hpp"
#include "window_impl.hpp"

#include <X11/Xlib.h>

namespace Gui {
class XWindowImpl: public WindowImpl
{
public:
    explicit XWindowImpl(const GlyphParams&);
    // TODO(rmn): mb make static creator
    explicit XWindowImpl(const GlyphParams&, XWindowImpl*);
    ~XWindowImpl() override;

    void DrawRectangle(const Point& point, const width_t width, const height_t height) override;
    void DrawText(const Point& text_position, std::string text) override;
    void DrawText(const GlyphParams& params, const std::string& text, Alignment alignment) override;
    void DrawLine(const Point& start_point, const Point& end_point) override;

    void SetForeground(const int color) override;

    void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color) override;

    void ShowWindow() override;
    void HideWindow() override;

    [[nodiscard]] unsigned long GetWindow() const override { return m_window; }
    [[nodiscard]] void* GetDisplay() const override { return m_display; }
    [[nodiscard]] ::GC GetGc() const { return m_gc; }

    void ClearWindow() override;
    void ClearGlyph(const GlyphParams& p, bool sendExposureEvent) override;
    void Resize(width_t width, height_t height) override;

    void SetFontPath(const std::string& path) override;

    std::set<Lexi::FontName> GetFontList() override;
    bool ChangeFont(Lexi::Font&) override;

private:
    void CreateWindow(const GlyphParams& params);
    void CreateGraphicContext();

    ::Display* m_display = nullptr;
    ::Window m_window = 0;
    ::GC m_gc = nullptr;

    bool m_isChild = false;
};
}  // namespace Gui

#endif  // LEXI_X_WINDOW_IMPL_CPP_HPP
