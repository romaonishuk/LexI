//
// Created by ronyshchuk on 14.11.20.
//
#include "i_composite_glyph.hpp"

ICompositeGlyph::ICompositeGlyph(const GlyphParams params): IGlyph(params) {}

void ICompositeGlyph::Draw(Gui::Window* window)
{
    for(auto& it: m_components) {
        it->Draw(window);
    }
}

void ICompositeGlyph::DrawAt(Gui::Window* window, const Point& point)
{
    for(auto& it: m_components) {
        it->DrawAt(window, point);
    }
}

void ICompositeGlyph::ProcessEvent(Gui::Window* w, const Event& event)
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

void ICompositeGlyph::Insert(GlyphPtr glyph, size_t pos)
{
    if(pos > m_components.size()) {
        m_components.push_back(std::move(glyph));
    } else {
        auto iter = m_components.begin();
        std::advance(iter, pos);
        m_components.insert(iter, std::move(glyph));
    }
}

void ICompositeGlyph::Add(GlyphPtr glyph) { m_components.push_back(std::move(glyph)); }

IGlyph::GlyphPtr ICompositeGlyph::Find(const Point& point)
{
    for(auto& it: m_components) {
        if(it->Intersects(point)) {
            return it;
        }
    }
    return nullptr;
}

void ICompositeGlyph::MoveGlyph(width_t x, height_t y) {
    IGlyph::MoveGlyph(x, y);
    for(auto&it: m_components) {
        it->MoveGlyph(x, y);
    }
}
