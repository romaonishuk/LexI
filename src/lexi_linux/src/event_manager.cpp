//
// Created by romaonishuk on 24.02.20.
//

#include <X11/Xlib.h>

#include <algorithm>
#include "event_manager.hpp"

#include "window.hpp"
#include "lexi_linux/inc/x_window_impl.hpp"

#include <iostream>

EventManager::EventManager(Gui::Window* w): m_mainWindow(w), m_currentWindow(m_mainWindow) {

}

bool EventManager::ChangeCurrentWindow(unsigned long window)
{
    if(window == m_currentWindow->m_window_impl->GetWindow()) {
        return true;
    }

    auto windowsIt = std::find_if(childWindows.begin(), childWindows.end(),
                          [window](const auto* w){return w->m_window_impl->GetWindow() == window;});
    if(windowsIt == childWindows.end()) {
        std::cout << "Didn't find appropriate window" << std::endl;
        return false;
    }

    m_currentWindow = *windowsIt;
    return true;
}

void EventManager::RunLoop()
{
    // TODO(rmn): redesign this
    auto* display = reinterpret_cast<::Display*>(m_mainWindow->m_window_impl->GetDisplay());

    while(!stopLoop)
    {
        XEvent event;
        XNextEvent(display, &event);

        Point p{static_cast<uint32_t>(event.xbutton.x), static_cast<uint32_t>(event.xbutton.y)};
        switch(event.type) {
            case Expose:
                if(ChangeCurrentWindow(event.xexpose.window)) {
                    m_currentWindow->Draw(m_currentWindow);
                }
                break;
            case MapNotify:
                break;
            case DestroyNotify:
                std::cout << "Close received!" << std::endl;
                return;
            case ButtonPress:
                if(ChangeCurrentWindow(event.xexpose.window)) {
                    m_currentWindow->ProcessEvent(m_currentWindow, p, EventType::ButtonPressed);
                }
                break;
            case ButtonRelease:
                m_currentWindow->ProcessEvent(m_currentWindow, p, EventType::ButtonReleased);
                break;
            case ClientMessage:
                // TODO(rmn): learn about client messages!
                std::cout << "Close received!" << std::endl;
                return ;
            case CreateNotify:
                break;
            case MotionNotify:
                std::cout << "Motion: x" << event.xmotion.x << " y: " << event.xmotion.y << std::endl;
                m_currentWindow->ProcessEvent(m_currentWindow, p, EventType::FocusedIn);
                break;
            default:
                std::cout << "RMN unprocessed event:" << event.type << std::endl;
                break;
        }
    }
}

void EventManager::Stop()
{
    std::cout << "Event loop was stopped!";
    stopLoop = true;
}
