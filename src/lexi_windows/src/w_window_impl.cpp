//
// Created by romaonishuk on 01.12.2020.
//

#include "w_window_impl.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "windows_helpers.h"

#include <Windows.h>


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

    ::ShowWindow(m_handler, SW_SHOWNORMAL);
}

WWindowImpl::~WWindowImpl()
{
    if(::DestroyWindow(m_handler)) {
        Logger::Get().Log("Failed to destroy window!");
    }
    Logger::Get().Log("Window closed!");
}

void WWindowImpl::DrawRectangle(const Point &point, const width_t width, const height_t height) {

}

void WWindowImpl::DrawText(const Point &text_position, std::string text)
{

}

void WWindowImpl::DrawText(const GlyphParams &params, const std::string &text, Alignment alignment)
{

}

void WWindowImpl::DrawLine(const Point &start_point, const Point &end_point) {}

void WWindowImpl::SetForeground(const int color) {}

void WWindowImpl::FillRectangle(const Point &point, const width_t width, const height_t height,
                   const Color color) {}

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
