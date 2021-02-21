//
// Created by romaonishuk on 01.12.2020.
//

#include "w_window_impl.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "windows_helpers.h"

#include <Windows.h>
#include <winuser.h>


// TODO(rmn): check if there any bad consequences
#undef DrawText



namespace Gui {
WWindowImpl::WWindowImpl(const GlyphParams& params)
{
    using Lexi::Windows::GetLastErrorAsString;

    auto hInstance = GetModuleHandle(nullptr);
    m_handler = ::CreateWindow(Lexi::Config::GetName(), Lexi::Config::GetName(),
            WS_OVERLAPPEDWINDOW, params.x, params.y, params.width, params.height,
            nullptr, nullptr, hInstance, this // LPVOID    lpParam
    );

    if(m_handler == nullptr) {
        std::cout << "Error during window creation: " << GetLastErrorAsString() << std::endl;
        throw std::runtime_error("Failed to create window!");
    }

    m_deviceCtx = ::GetDC(m_handler);
    if(!m_deviceCtx) {
        std::cout << "Failed to get device context: " << GetLastErrorAsString() << std::endl;
        throw std::runtime_error("Failed to get device context!");
    }

    ::ShowWindow(m_handler, SW_SHOWNORMAL);
}

WWindowImpl::~WWindowImpl()
{
    if(::DestroyWindow(m_handler)) {
        Logger::Get().Log("Failed to destroy window!");
    }
    Logger::Get().Log("Window closed!");
}

void WWindowImpl::DrawRectangle(const Point &point, const width_t width, const height_t height)
{
    if(::Rectangle(m_deviceCtx, point.x, point.y, width, height) == 0) {
        std::cout << "Failed to draw!" << std::endl;
    }
}

void WWindowImpl::DrawText(const Point &text_position, std::string text)
{
    RECT rect {text_position.x, text_position.y, 100, 100};
    ::DrawTextA(m_deviceCtx, text.c_str(), text.length(), &rect, DT_LEFT);
}

void WWindowImpl::DrawText(const GlyphParams &params, const std::string &text, Alignment alignment)
{
    RECT rect {params.x, params.y, params.width, params.height};
    ::DrawTextA(m_deviceCtx, text.c_str(), text.length(), &rect, DT_LEFT);
}

void WWindowImpl::DrawLine(const Point &start_point, const Point &end_point)
{
    ::MoveToEx(m_deviceCtx, end_point.x, end_point.y, nullptr);
    LineTo(m_deviceCtx, end_point.x, end_point.y);
}

void WWindowImpl::SetForeground(const int color)
{
    ::SetDCBrushColor(m_deviceCtx, color);
}

void WWindowImpl::FillRectangle(const Point &point, const width_t width, const height_t height, const Color color)
{
//    RECT rect {point.x, point.y, width, height};
//    ::FillRect(m_deviceCtx, &rect, 0); //static_cast<HBRUSH>(color));
}

void WWindowImpl::ShowWindow()
{
    ::ShowWindow(m_handler, false);
}

void WWindowImpl::HideWindow() {}

void WWindowImpl::ClearWindow() {}

void WWindowImpl::ClearGlyph(const GlyphParams &p, bool sendExposureEvent) {}

void WWindowImpl::Resize(width_t width, height_t height) {}

void WWindowImpl::SetFontPath(const std::string &path) {}

std::set<Lexi::FontName> WWindowImpl::GetFontList() {return {Lexi::FontName{"", ""}};}

bool WWindowImpl::ChangeFont(Lexi::Font &) {return false;}

unsigned long WWindowImpl::GetWindow() const  {return 0;};

void* WWindowImpl::GetDisplay() const  {return nullptr;};

}
