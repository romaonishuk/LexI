//
// Created by ronyshchuk on 03.04.20.
//

#include "font.hpp"

#include "config.hpp"

// --- Font Manager ---
namespace Lexi {
FontManager& FontManager::Get()
{
    static FontManager manager;
    return manager;
}

void FontManager::Init(Gui::Window* w)
{
    impl = w->GetImpl();
    impl->SetFontPath(Lexi::Config::GetFontsPath());

    m_existingFonts = impl->GetFontList();
    std::cout << "Successfully loaded " << m_existingFonts.size() << " fonts!" << std::endl;
}

bool FontManager::SetFont(Font& font)
{
    if(impl->ChangeFont(font)) {
        std::cout << "Font changed from " << m_currentFont.m_name << " to " << font.m_name << std::endl;
        m_currentFont = font;
        return true;
    }
    return false;
}

// Inherit current font settings
bool FontManager::SetFont(const std::string& fontName)
{
    auto font = m_currentFont;
    font.m_name = fontName;
    return SetFont(font);
}

void FontManager::SetFontWeight(const Font::Weight& w)
{
    m_currentFont.m_weight = w;
}
void FontManager::SetFontSlant(const Font::Slant& s)
{
    m_currentFont.m_slant = s;
}

void FontManager::SetFontSize(const std::string& fontSize)
{
    auto font = m_currentFont;
    font.m_fontSize = std::stoi(fontSize);
    SetFont(font);
}

std::vector<std::string> FontManager::GetFontList()
{
    return {m_existingFonts.begin(), m_existingFonts.end()};
}
}  // namespace Lexi
