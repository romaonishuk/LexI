//
// Created by ronyshchuk on 31.03.20.
//

#ifndef LEXI_FONT_HPP
#define LEXI_FONT_HPP

#include "common.hpp"
#include "window.hpp"
#include "window_impl.hpp"

#include <set>

namespace {
const auto path = "/home/ronyshchuk/Desktop/LexI/resources/fonts";
}

namespace Lexi {
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

struct Font
{
    enum class Weight
    {
        kMedium,
        kBold
    };

    enum class Slant
    {
        kNormal,
        kItalic
    };

    Font() = default;
    Font(const std::string& name, unsigned long fid): m_name(name), m_fontId(fid) {}

    std::string m_name;
    unsigned long m_fontId = 0;

    uint32_t m_fontSize = 14;
    Weight m_weight = Weight::kMedium;
    Slant m_slant = Slant::kNormal;
};

class FontManager
{
public:
    static FontManager& Get();

    void Init(Gui::Window* w);

    bool SetFont(const std::string& fontName);
    bool SetFont(Font& font);
    void SetFontWeight(const Font::Weight& w);
    void SetFontSlant(const Font::Slant& s);
    void SetFontSize(const std::string& fontSize);
    unsigned long GetFontId() { return m_currentFont.m_fontId; };

    std::vector<std::string> GetFontList();

private:
    FontManager() = default;
    ~FontManager() = default;

    Gui::WindowImpl* impl = nullptr;
    Font m_currentFont;
    std::set<FontName> m_existingFonts;
};
}  // namespace Lexi

#endif  // LEXI_FONT_HPP
