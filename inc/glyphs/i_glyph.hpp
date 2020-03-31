//
// Created by romaonishuk on 28.10.19.
//

#ifndef LEXI_I_GLYPH_HPP
#define LEXI_I_GLYPH_HPP

#include <memory>

#include "types.hpp"

namespace Gui {
class Window;
}

class IGlyph
{
public:
    explicit IGlyph(const GlyphParams& params): m_params(params) {}
    IGlyph(unsigned int x, unsigned int y, width_t w, height_t h): m_params({x, y, w, h}) {}

    using GlyphPtr = std::shared_ptr<IGlyph>;
    virtual void Draw(Gui::Window*) = 0;  // TODO: const?
    virtual void Insert(GlyphPtr, size_t) {}

    width_t GetWidth() const { return m_params.width; }

    height_t GetHeight() const { return m_params.height; }

    bool Intersects(const Point& p) { return m_params.IsIntersects(p); }

    virtual void ProcessEvent(Gui::Window* w, const Point& p, const EventType& ev) = 0;

    virtual void Add(GlyphPtr glyph) {}

    virtual ~IGlyph() = default;

protected:
    GlyphParams m_params;
};

#endif  // LEXI_I_GLYPH_HPP
