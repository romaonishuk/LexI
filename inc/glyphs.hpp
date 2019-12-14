//
// Created by romaonishuk on 28.10.19.
//

#ifndef LEXI_I_GLYPH_HPP
#define LEXI_I_GLYPH_HPP

#include <memory>
#include <list>
#include <string>

#include "types.hpp"

namespace Gui {
class Window;
}

class IGlyph
{
public:
    explicit IGlyph(const GlyphParams params) : m_params(params){}
    IGlyph(unsigned int x, unsigned int y, width_t w, height_t h) : m_params({x, y, w, h}){}

    using GlyphPtr = std::unique_ptr<IGlyph>;
    virtual void Draw(Gui::Window *) = 0;  // TODO: const?
    virtual void Insert(GlyphPtr, size_t ) {

    }

    virtual void Add(GlyphPtr glyph){}

    virtual ~IGlyph() = default;
protected:
    GlyphParams m_params;
};

class ICompositeGlyph: public IGlyph
{
public:
    explicit ICompositeGlyph(const GlyphParams params): IGlyph(params) {}
    void Draw(Gui::Window *window) override {
        for(auto &it: m_components) {
            it->Draw(window);
        }
    }

    virtual ~ICompositeGlyph() = default;

    void Insert(GlyphPtr glyph, size_t pos) override {
        // TODO: test me!
        if(pos > m_components.size()) {
            m_components.push_back(std::move(glyph));
        } else {
            auto iter = m_components.begin();
            std::advance(iter, pos);
            m_components.insert(iter, std::move(glyph));
        }
    }

    void Add(std::unique_ptr<IGlyph> glyph) override {
        m_components.push_back(std::move(glyph));
    }

//    void Remove();

protected:
    std::list<std::unique_ptr<IGlyph>> m_components;
};

#endif //LEXI_I_GLYPH_HPP
