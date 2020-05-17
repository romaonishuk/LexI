//
// Created by romaonishuk on 04.03.20.
//

#ifndef LEXI_I_COMPOSITE_GLYPH_HPP
#define LEXI_I_COMPOSITE_GLYPH_HPP

#include <list>
#include <string>

#include "i_glyph.hpp"

/**
 * \brief  The class represents a complex glyph, i.e glyph that contains one or more glyphs.
 *         Provides basic functionality to work with a list of IGlyphs.
 *
 * \attention  Implements Composite pattern.
 */
class ICompositeGlyph: public IGlyph
{
public:
    using GlyphList = std::list<GlyphPtr>;

    explicit ICompositeGlyph(const GlyphParams params): IGlyph(params) {}

    void Draw(Gui::Window* window) override
    {
        for(auto& it: m_components) {
            it->Draw(window);
        }
    }

    void DrawAt(Gui::Window* window, const Point& point) override
    {
        for(auto& it: m_components) {
            it->DrawAt(window, point);
        }
    }

    ~ICompositeGlyph() override = default;

    void Insert(GlyphPtr glyph, size_t pos) override
    {
        if(pos > m_components.size()) {
            m_components.push_back(std::move(glyph));
        } else {
            auto iter = m_components.begin();
            std::advance(iter, pos);
            m_components.insert(iter, std::move(glyph));
        }
    }

    void ProcessEvent(Gui::Window* w, const Event& event) override
    {
        const auto& point = event.GetPoint();
        if(Intersects(point)) {
            for(const auto& it: m_components) {
                if(it->Intersects(point)) {
                    return it->ProcessEvent(w, event);
                }
            }
        }
    }

    void Add(GlyphPtr glyph) override { m_components.push_back(std::move(glyph)); }

    //    void Remove();
    GlyphPtr Find(const Point& point)
    {
        for(auto& it: m_components) {
            if(it->Intersects(point)) {
                return it;
            }
        }
        return nullptr;
    }

protected:
    GlyphList m_components;
};

#endif  // LEXI_I_COMPOSITE_GLYPH_HPP
