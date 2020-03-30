//
// Created by romaonishuk on 10.03.20.
//

#ifndef LEXI_MENU_HPP
#define LEXI_MENU_HPP

// TODO(rmn): need to investigate how this will be implemented on another OS
#include "window.hpp"

class ICommand;

namespace Gui
{
class Menu : public ICompositeGlyph
{
public:
    explicit Menu(const GlyphParams& p, const std::string& title, Window* w);

    void Draw(Window * w) override;

    void ProcessEvent(Window *w,const Point& p, const EventType& ev) override;

    void Add(GlyphPtr glyph) override;

    ChildWindow* getMenuWindow() {
        return mMenuWindow.get();
    }
private:
    std::unique_ptr<ChildWindow> mMenuWindow = nullptr;
    std::string m_title;
};
}  // namespace Gui

#endif //LEXI_MENU_HPP
