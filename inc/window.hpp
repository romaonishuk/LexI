//
// Created by romaonishuk on 27.10.19.
//

#ifndef LEXI_WINDOW_HPP
#define LEXI_WINDOW_HPP

#include <memory>

#include "i_composite_glyph.hpp"
#include "i_glyph.hpp"
// TODO: WindowImpl should be private for Window
#include "types.hpp"
#include "window_impl.hpp"

#include "event_manager.hpp"

namespace Gui {
class ChildWindow;

class Window: public ICompositeGlyph
{
public:
    explicit Window(const GlyphParams&);
    Window(const GlyphParams&, Window*);
    ~Window() override;

    void Draw(Gui::Window*) override;
    void ReDraw();
    void DrawRectangle(const Point& point, const width_t width, const height_t height) const;
    void DrawText(const Point& text_position, std::string text) const;
    void DrawText(const GlyphParams&, const std::string&, Alignment);
    void DrawLine(const Point& start_point, const Point& end_point) const;

    void FillRectangle(const Point& point, const width_t width, const height_t height, const Color color);

    void SetForeground(int color) const;

    void ProcessEvent(Gui::Window* w, const Point& p, const EventType& ev) override
    {
        for(const auto& it: m_components) {
            if(it->Intersects(p)) {
                return it->ProcessEvent(w, p, ev);
            }
        }
    }
    void ClearArea(const GlyphParams& p) { m_window_impl->ClearGlyph(p, false); }

    // TODO(rmn): tempo until graphic context investigation
    WindowImpl* GetImpl() { return m_window_impl.get(); }

    void ShowWindow() const;
    void HideWindow() const;

protected:
    friend EventManager;
    std::unique_ptr<WindowImpl> m_window_impl;
};

// TODO(rmn): refactor this. mb rename to menuWindow
class ChildWindow: public Window
{
public:
    ChildWindow(const GlyphParams&, Window*);
    void Draw(Window*) override;
    void ProcessEvent(Gui::Window* w, const Point& p, const EventType& ev) override;
    void Resize(width_t width, height_t height);

    void SetCurrentMenuItem(GlyphPtr item) {
        m_currentMenuItem = std::move(item);
    }
private:
    // TODO(rmn): weak_ptr
    Window* m_parent;
    GlyphPtr m_currentMenuItem = nullptr;
};
}  // namespace Gui

#endif  // LEXI_WINDOW_HPP
