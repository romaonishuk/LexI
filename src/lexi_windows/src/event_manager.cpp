//
// Created by romaonishuk on 03.12.2020.
//

#include "event_manager.hpp"

#include <iostream>
#include <windows.h>

#include "config.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_NCCREATE:
        case WM_GETMINMAXINFO:
        case WM_NCCALCSIZE:
            return DefWindowProc(hwnd, msg, wParam, lParam);
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);



            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        break;
        default:
            std::cout << "Event: " << std::hex << msg << std::endl;
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return true;
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
