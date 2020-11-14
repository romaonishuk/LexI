//
// Created by romaonishuk on 04.03.20.
//

#ifndef LEXI_I_COMPOSITE_GLYPH_HPP
#define LEXI_I_COMPOSITE_GLYPH_HPP

#include <list>
#include <algorithm>
#include <cassert>

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

    explicit ICompositeGlyph(const GlyphParams params);
    ~ICompositeGlyph() override = default;

    void Draw(Gui::Window* window) override;
    void DrawAt(Gui::Window* window, const Point& point) override;

    void ProcessEvent(Gui::Window* w, const Event& event) override;

    void Add(GlyphPtr glyph) override;
    void Insert(GlyphPtr glyph, size_t pos) override;

    /**
     * Insert @p glyphToInsert after @p glyph
     */
    template <typename T>
    void InsertAfter(const std::shared_ptr<T>& glyph, const std::shared_ptr<T>& glyphToInsert)
    {
        auto res = std::find_if(m_components.begin(), m_components.end(), [&](const auto& it){
          return it.get() == glyph.get();
        });
        assert(res != m_components.end());
        res = std::next(res);
        m_components.insert(res, glyphToInsert);
    }

    //    void Remove();
    GlyphPtr Find(const Point& point);

    void MoveGlyph(width_t x, height_t y) override;

protected:
    GlyphList m_components;
};

#endif  // LEXI_I_COMPOSITE_GLYPH_HPP
