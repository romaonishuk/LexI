//
// Created by ronyshchuk on 31.03.20.
//

#ifndef LEXI_FONT_HPP
#define LEXI_FONT_HPP

#include "common.hpp"
#include "window.hpp"
#include "window_impl.hpp"

#include <iostream>
#include <set>

namespace {
const auto path = "/home/ronyshchuk/Desktop/LexI/resources/fonts";
}

struct FontName
{
    FontName(const std::string& name, const std::string& additionalName):
        m_fontName(name),
        m_addStyleName(additionalName)
    {}

    bool operator<(const FontName& f) const { return !(*this == f); }

    bool operator==(const FontName& f) const
    {
        return m_fontName == f.m_fontName && m_addStyleName == f.m_addStyleName;
    }

    operator std::string() const { return m_addStyleName.empty() ? m_fontName : m_fontName + " " + m_addStyleName; }

    bool IsEmpty() const { return m_fontName.empty(); }

    std::string m_fontName;
    std::string m_addStyleName;
};

struct FontInfo
{
    FontInfo() = default;
    FontInfo(const std::string& name, unsigned long fid): m_name(name), m_fontId(fid) {}

    std::string m_name;
    unsigned long m_fontId = 0;

    uint32_t font_size = 12;
};

class FontManager
{
public:
    static FontManager& Get()
    {
        static FontManager manager;
        return manager;
    }

    void Init(Gui::Window* w)
    {
        impl = w->GetImpl();
        impl->SetFontPath(path);

        m_existingFonts = impl->GetFontList();
        std::cout << "Successfully loaded " << m_existingFonts.size() << " fonts!" << std::endl;
    }

    void ChangeFont(const std::string& fontName) {
        auto font = impl->ChangeFont(fontName);
        if(font) {
            std::cout << "Font changed from " << m_currentFont.m_name << " to " << font->m_name << std::endl;
            m_currentFont = *font;
        }
    }

    std::vector<std::string> GetFontList() { return {m_existingFonts.begin(), m_existingFonts.end()}; }

private:
    FontManager() = default;
    ~FontManager() = default;

    Gui::WindowImpl* impl = nullptr;
    FontInfo m_currentFont;
    std::set<FontName> m_existingFonts;
};

#endif  // LEXI_FONT_HPP
