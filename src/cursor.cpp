//
// Created by ronyshchuk on 07.04.20.
//

#include "cursor.hpp"
#include "window.hpp"

// Cursor::Cursor(const GlyphParams& params, Gui::Window* w): IGlyph(params), m_currentWindow(w)
//{
////    constexpr auto interval = 500;
////    m_t = std::thread([this, &interval] {
////        std::cout << "Thread started!" << std::endl;
////        bool isVisible = true;
////        while(!stopThread) {
////            DrawCursor(isVisible);
////            isVisible = !isVisible;
////            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
////        }
////        std::cout << "Thread stopped" << std::endl;
////    });
//}
namespace Lexi {
void Cursor::MoveCursor(Gui::Window* newWindow, const GlyphParams& newParams)
{
    // Clean previous cursor location
    if(m_currentWindow) {
        m_currentWindow->SetForeground(kWhite);
        m_currentWindow->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});
        m_currentWindow->SetForeground(kBlack);
    }

//    std::cout << "Moving cursor from: " << m_params << " to: " << newParams << std::endl;

    m_currentWindow = newWindow;
    m_params = newParams;
    DrawCursor(true);
}

void Cursor::DrawCursor(bool visible)
{
    if(visible) {
        m_currentWindow->SetForeground(kBlack);
        m_currentWindow->DrawLine({m_params.x, m_params.y}, {m_params.x, m_params.y + m_params.height});
        m_currentWindow->SetForeground(kWhite);
    } else {
        m_currentWindow->FillRectangle({m_params.x, m_params.y}, m_params.width, m_params.height, Color::kWhite);
    }
}

void Cursor::Draw(Gui::Window* w)
{
    DrawCursor(true);
}
}  // namespace Lexi