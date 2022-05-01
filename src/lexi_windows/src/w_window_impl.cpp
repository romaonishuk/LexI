//
// Created by romaonishuk on 01.12.2020.
//

#include "w_window_impl.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "windows_helpers.h"

#include <Windows.h>
#include <winuser.h>


namespace {
uint32_t ConvertAlignment(Alignment alignment) {
    switch(alignment) {
        case Alignment::kCenter:
            return DT_SINGLELINE | DT_VCENTER | DT_CENTER;
        case Alignment::kLeft:
            return DT_LEFT;
        case Alignment::kRight:
        default:
            std::cout << "Unsupported alignment" << std::endl;
            break;
    }
    return DT_SINGLELINE | DT_VCENTER | DT_CENTER;
}
}

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

WWindowImpl::WWindowImpl(const GlyphParams& params, WWindowImpl* parentImpl)
{
    using Lexi::Windows::GetLastErrorAsString;

    m_isChild = true;
    auto hInstance = GetModuleHandle(nullptr);
    m_handler = ::CreateWindow(Lexi::Config::GetName(), "Child window",
        WS_POPUP, params.x, params.y, params.width, params.height,
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

    ::SetParent(m_handler, parentImpl->m_handler);
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
    if(::Rectangle(m_deviceCtx, point.x, point.y, point.x + width, point.y + height) == 0) {
        std::cout << "Failed to draw!" << std::endl;
    }
}

void WWindowImpl::DrawText(const Point &text_position, std::string text)
{
    // TODO(rmn): fix this after fonts clarification
    RECT rect {text_position.x, text_position.y - 10, text_position.x + 100, text_position.y};
    if(::DrawTextA(m_deviceCtx, text.c_str(), text.length(), &rect, ConvertAlignment(Alignment::kCenter)) == 0) {
        std::cout << "Failed to draw text: " << text << std::endl;
    }
}

void WWindowImpl::DrawText(const GlyphParams &params, const std::string &text, Alignment alignment)
{
    RECT rect {params.x, params.y, params.x + params.width, params.y + params.height};
    if(::DrawTextA(m_deviceCtx, text.c_str(), text.length(), &rect, ConvertAlignment(alignment)) == 0) {
        std::cout << "Failed to draw text: " << text << std::endl;
    }
}

void WWindowImpl::DrawLine(const Point &start_point, const Point &end_point)
{
    ::MoveToEx(m_deviceCtx, start_point.x, start_point.y, nullptr);
    LineTo(m_deviceCtx, end_point.x, end_point.y);
}

void WWindowImpl::SetForeground(const int color)
{
    ::SetDCBrushColor(m_deviceCtx, color);
}

void WWindowImpl::FillRectangle(const Point &point, const width_t width, const height_t height, const Color color)
{
    RECT rect {point.x, point.y, point.x + width, point.y + height};
    auto brush = ::CreateSolidBrush(color);
    if(::FillRect(m_deviceCtx, &rect, brush) == 0) {
        std::cout << "Failed to fill rectangle at: " << point << std::endl;
    }
    DeleteObject(brush);  // TODO(rmn): investigate what is brush for
}

void WWindowImpl::ShowWindow()
{
    ::ShowWindow(m_handler, SW_SHOWNORMAL);
}

void WWindowImpl::HideWindow()
{
    ::ShowWindow(m_handler, SW_HIDE);
}

void WWindowImpl::ClearWindow()
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
}

void WWindowImpl::ClearGlyph(const GlyphParams &p, bool sendExposureEvent)
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
}

void WWindowImpl::Resize(width_t width, height_t height)
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
}

void WWindowImpl::SetFontPath(const std::string &path)
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
}

std::set<Lexi::FontName> WWindowImpl::GetFontList() {return {Lexi::FontName{"", ""}};}

bool WWindowImpl::ChangeFont(Lexi::Font &)
{
//    WM_SETFONT(NULL, true);
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
    return false;
}

unsigned long WWindowImpl::GetWindow() const
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
    return 0;
}

void* WWindowImpl::GetDisplay() const
{
    std::cout << __FUNCTION__ << " not yet implemented!" << std::endl;
    return nullptr;
}
}
