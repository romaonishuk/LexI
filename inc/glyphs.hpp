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

    using GlyphPtr = std::shared_ptr<IGlyph>;
    virtual void Draw(Gui::Window *) = 0;  // TODO: const?
    virtual void Insert(GlyphPtr, size_t ) {

    }

    bool Intersects(const Point& p) {
        return m_params.IsIntersects(p);
    }

    virtual void ProcessEvent(Gui::Window *w, const Point& p, const EventType& ev) = 0;

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

    void ProcessEvent(Gui::Window *w,const Point& p, const EventType& ev)
    {
        if(Intersects(p)) {
            for(const auto& it: m_components) {
                if(it->Intersects(p)) {
                    return it->ProcessEvent(w, p, ev);
                }
            }
        }
    }

    void Add(GlyphPtr glyph) override {
        m_components.push_back(std::move(glyph));
    }

//    void Remove();

protected:
    std::list<GlyphPtr> m_components;
};

#endif //LEXI_I_GLYPH_HPP
