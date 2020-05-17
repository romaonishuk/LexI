//
// Created by romaonishuk on 28.10.19.
//

#ifndef LEXI_I_GLYPH_HPP
#define LEXI_I_GLYPH_HPP

#include <memory>

// TODO(rmn): remove after logger finished
#include <iostream>

#include "events.hpp"
#include "types.hpp"

namespace Gui {
class Window;
}

using Lexi::Event;
using Lexi::EventType;

/**
 * \brief  Base class for all graphical units in the project.
 */
class IGlyph
{
public:
    explicit IGlyph(const GlyphParams& params): m_params(params) {}
    IGlyph(int x, int y, width_t w, height_t h): m_params({x, y, w, h}) {}
    virtual ~IGlyph() = default;

    using GlyphPtr = std::shared_ptr<IGlyph>;

    virtual void Draw(Gui::Window*) = 0;  // TODO: const?
    /**
     * @brief           Draw current glyph at specific position.
     * @param point     New position of glyph.
     */
    virtual void DrawAt(Gui::Window* w, const Point& point) { Draw(w); }
    virtual void ReDraw(Gui::Window* w)
    {
        ClearGlyph(w);
        Draw(w);
    }
    virtual void Insert(GlyphPtr, size_t) {}

    bool Intersects(const Point& p) { return m_params.IsIntersects(p); }

    virtual void ProcessEvent(Gui::Window* w, const Event& event) = 0;

    virtual void Add(GlyphPtr glyph) {}

    void ClearGlyph(Gui::Window* w);

    void SetPosition(const Point& p)
    {
        m_params.x = p.x;
        m_params.y = p.y;
    }

    void SetPosition(width_t x, height_t y)
    {
        m_params.x = x;
        m_params.y = y;
    }

    void SetGlyphParams(const GlyphParams& params) { m_params = params; }
    const GlyphParams& GetGlyphParams() const { return m_params; }
    width_t GetWidth() const { return m_params.width; }
    height_t GetHeight() const { return m_params.height; }
    Point GetPosition() const { return {m_params.x, m_params.y}; }
    [[nodiscard]] height_t GetBottomBorder() const noexcept { return m_params.y + m_params.height; }
    [[nodiscard]] height_t GetRightBorder() const noexcept { return m_params.x + m_params.width; }

protected:
    GlyphParams m_params;
};

#endif  // LEXI_I_GLYPH_HPP
