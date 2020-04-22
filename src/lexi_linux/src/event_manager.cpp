//
// Created by romaonishuk on 24.02.20.
//

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <algorithm>
#include "event_manager.hpp"

#include "cursor.hpp"
#include "lexi_linux/inc/x_window_impl.hpp"
#include "window.hpp"

#include <iostream>

EventManager::EventManager(Gui::Window* w): m_mainWindow(w), m_currentWindow(m_mainWindow)
{}

bool EventManager::ChangeCurrentWindow(unsigned long window)
{
    if(window == m_currentWindow->m_window_impl->GetWindow()) {
        return true;
    }

    auto windowsIt = std::find_if(childWindows.begin(), childWindows.end(),
        [window](const auto* w) { return w->m_window_impl->GetWindow() == window; });
    if(windowsIt == childWindows.end()) {
        std::cout << "Didn't find appropriate window" << std::endl;
        return false;
    }

    std::cout << "Window changed from: " << m_currentWindow->GetWindowName() << " to: " << (*windowsIt)->GetWindowName()
              << std::endl;
    m_currentWindow = *windowsIt;
    return true;
}

void EventManager::RunLoop()
{
    using namespace Lexi;
    // TODO(rmn): redesign this
    auto* display = reinterpret_cast<::Display*>(m_mainWindow->m_window_impl->GetDisplay());
    while(!stopLoop) {
        XEvent event;
        XNextEvent(display, &event);

        Point p{event.xbutton.x, event.xbutton.y};
        switch(event.type) {
            case Expose:
                if(ChangeCurrentWindow(event.xexpose.window)) {
                    m_currentWindow->Draw(m_currentWindow);
                }
                break;
            case MapNotify:
            case UnmapNotify:
                break;
            case DestroyNotify:
                std::cout << "Close received!" << std::endl;
                return;
            case ButtonPress:
                if(ChangeCurrentWindow(event.xbutton.window)) {
                    switch(event.xbutton.button) {
                        case Button1:
                            m_currentWindow->ProcessEvent(m_currentWindow, {p, EventType::MouseButtonPressed});
                            break;
                        case Button2:
                            break;
                        case Button3:
                            break;
                        case Button4:
                            m_currentWindow->ProcessEvent(m_currentWindow, ScrollEvent{p, ScrollEvent::Direction::kUp});
                            break;
                        case Button5:
                            m_currentWindow->ProcessEvent(
                                m_currentWindow, ScrollEvent{p, ScrollEvent::Direction::kDown});
                            break;
                        default:
                            break;
                    }
                }
                break;
            case ButtonRelease:
                if(m_currentWindow->m_window_impl->GetWindow() == event.xbutton.window) {
                    m_currentWindow->ProcessEvent(m_currentWindow, {p, EventType::MouseButtonReleased});
                }
                break;
            case ClientMessage:
                // TODO(rmn): learn about client messages!
                std::cout << "Close received!" << std::endl;
                return;
            case CreateNotify:
                break;
            case MotionNotify:
                //                std::cout << "X: " << p.x << " Y: " << p.y << std::endl;
                m_currentWindow->ProcessEvent(m_currentWindow, {p, EventType::MouseMotion});
                break;
            case FocusIn:
                std::cout << "Focus in" << std::endl;
                break;
            case FocusOut:
                std::cout << "Focus out" << std::endl;
                break;
            case KeyPress: {
                auto mykey = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, event.xkey.state & ShiftMask ? 1 : 0);
                if(mykey == NoSymbol) {
                    std::cout << "Wrong Key:" << std::endl;
                } else {
                    // If cursor is active redirect key press action to it
                    // TODO(rmn): need stabilization
                    m_currentWindow->ProcessEvent(
                        m_currentWindow, Lexi::KeyBoardEvent(Lexi::Cursor::Get().GetPosition(), mykey));
                }
            } break;
            case KeyRelease:
            case EnterNotify:
            case LeaveNotify:
            case ConfigureNotify:
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
