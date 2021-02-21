//
// Created by romaonishuk on 03.12.2020.
//

#include "event_manager.hpp"

#include "window.hpp"
#include <iostream>
#include <windows.h>
#include "config.hpp"

static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto currentWindow = EventManager::Get().GetCurrentWindow();
    switch (msg)
    {
        case WM_NCCREATE:
        case WM_GETMINMAXINFO:
        case WM_NCCALCSIZE:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        case WM_SHOWWINDOW:
            break;
        case WM_CREATE:
            std::cout << "WM_CREATE!" << std::endl;
            break;
        case WM_DESTROY:
            std::cout << "WM_DESTROY!" << std::endl;
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            std::cout << "Event: " << "WM_SIZE" << std::endl;
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT  lpPaint;
            if(auto hdc = BeginPaint(hwnd, &lpPaint)) {
//                const auto& rc = lpPaint.rcPaint;
//                Lexi::DrawEvent draw({rc.left, rc.top}, rc.right, rc.bottom);
                if(currentWindow) {
                    currentWindow->Draw(currentWindow);
                }

                EndPaint(hwnd, &lpPaint);
            }
        }
            break;
        default:
            std::cout << "Event: " << std::hex << msg << std::endl;
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return true;
}

EventManager& EventManager::Get()
{
    static EventManager inst;
    return inst;
}

EventManager::EventManager()
{
    auto hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = Lexi::Config::GetName();

    RegisterClass(&wc);
}

void EventManager::RunLoop()
{
    MSG msg;
    while(!stopLoop && GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void EventManager::Stop()
{
    stopLoop = true;
}
