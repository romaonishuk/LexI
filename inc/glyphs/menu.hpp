//
// Created by romaonishuk on 10.03.20.
//

#ifndef LEXI_MENU_HPP
#define LEXI_MENU_HPP

// TODO(rmn): need to investigate how this will be implemented on another OS
#include "window.hpp"

class ICommand;

namespace Gui {
class MenuWindow: public Window
{
public:
    MenuWindow(const GlyphParams&, Window*);
    void Draw(Window*) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override;
    void Resize(width_t width, height_t height);

    void SetCurrentMenuItem(GlyphPtr item) { m_currentMenuItem = std::move(item); }

    [[nodiscard]] std::string GetWindowName() const override { return "Child Window"; }

private:
    // TODO(rmn): weak_ptr
    Window* m_parent;
    GlyphPtr m_currentMenuItem = nullptr;
};

class Menu: public ICompositeGlyph
{
public:
    explicit Menu(const GlyphParams& p, const std::string& title, Window* w);

    void Draw(Window* w) override;
    void ProcessEvent(Gui::Window* w, const Event& event) override;

    MenuWindow* getMenuWindow() { return mMenuWindow.get(); }

protected:
    void InitMenuWindow();
    // TODO(rmn) private:
    std::unique_ptr<MenuWindow> mMenuWindow = nullptr;
    std::string m_title;
};

class DropDownMenu: public Menu
{
public:
    DropDownMenu(const GlyphParams& p, const std::string& title, Window* w): Menu(p, title, w) {}
    void Draw(Window* w) override;
};

}  // namespace Gui

#endif  // LEXI_MENU_HPP
