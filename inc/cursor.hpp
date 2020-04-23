//
// Created by ronyshchuk on 07.04.20.
//

#ifndef LEXI_CURSOR_HPP
#define LEXI_CURSOR_HPP

#include "i_glyph.hpp"

//#include <thread>
namespace Lexi {
class Cursor
{
public:
    static Cursor& Get()
    {
        static Cursor c;
        return c;
    }
    void MoveCursor(Gui::Window* newWindow, const GlyphParams& newParams);
    void Draw(Gui::Window* w);
    [[nodiscard]] Gui::Window* GetCurrentWindow() const { return m_currentWindow; }
    [[nodiscard]] Point GetPosition() const { return {m_params.x, m_params.y}; }
    bool IsActive() {return true;}
    // TODO(rmn): fixfix
    height_t GetCursorEnd() const { return m_params.y + m_params.height; }

private:
    Cursor() = default;
    ~Cursor() = default;

    void DrawCursor(bool visible);

    Gui::Window* m_currentWindow = nullptr;
    GlyphParams m_params;

    //    bool stopThread = false;
    //    std::thread m_t;
};
}  // namespace Lexi

#endif  // LEXI_CURSOR_HPP
