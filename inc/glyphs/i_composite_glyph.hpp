//
// Created by romaonishuk on 04.03.20.
//

#ifndef LEXI_I_COMPOSITE_GLYPH_HPP
#define LEXI_I_COMPOSITE_GLYPH_HPP

#include <list>
#include <string>

#include "i_glyph.hpp"

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

#endif //LEXI_I_COMPOSITE_GLYPH_HPP