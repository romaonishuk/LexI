//
// Created by romaonishuk on 24.02.20.
//

#include "event_manager.hpp"

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#include <algorithm>
#include <iostream>

#include "cursor.hpp"
#include "x_window_impl.hpp"
#include "window.hpp"

EventManager::EventManager()
{}

bool EventManager::ChangeCurrentWindow(unsigned long window)
{
    if(!m_currentWindow) {
        m_currentWindow = m_mainWindow;
        return true;
    }

    if(window == m_currentWindow->m_window_impl->GetWindow()) {
        return false;
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
                break;
            case UnmapNotify:
                m_currentWindow->Draw(m_currentWindow);
                break;
            case DestroyNotify:
                std::cout << "Close received!" << std::endl;
                return;
            case ButtonPress: {
                auto* previousWindow = m_currentWindow;
                bool isWindowChanged = ChangeCurrentWindow(event.xbutton.window);
                switch(event.xbutton.button) {
                    case Button1:
                        m_currentWindow->ProcessEvent(m_currentWindow, {p, EventType::MouseButtonPressed});
                        break;
                    case Button2:
                        break;
                    case Button3:
                        break;
                    case Button4:
                        ProcessCursorRelatedEvent(ScrollEvent{p, ScrollEvent::Direction::kUp});
                        break;
                    case Button5:
                        ProcessCursorRelatedEvent(ScrollEvent{p, ScrollEvent::Direction::kDown});
                        break;
                    default:
                        break;
                }

                if(isWindowChanged && previousWindow->IsHideOnSwitch()) {
                    previousWindow->HideWindow();
                }

            } break;
            case ButtonRelease:
                // TODO(rmn): investigate cuvettes
                if(event.xbutton.window == m_currentWindow->m_window_impl->GetWindow()) {
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
                m_currentWindow->ProcessEvent(m_currentWindow, {p, EventType::MouseMotion});
                break;
            case KeyPress: {
                auto key = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, event.xkey.state & ShiftMask ? 1 : 0);
                if(key == NoSymbol) {
                    std::cout << "Wrong Key:" << std::endl;
                } else {
                    ProcessCursorRelatedEvent(Lexi::KeyBoardEvent(Lexi::Cursor::Get().GetPosition(), key));
                }
            } break;
            case KeyRelease:
                // TODO(rmn): add shortcuts and key buffer, which will be released on keyRelease!
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

void EventManager::ProcessCursorRelatedEvent(const Lexi::Event& event)
{
    // If cursor is active redirect key press action to it
    auto& cursor = Lexi::Cursor::Get();
    if(cursor.IsActive()) {
        auto* cursorWindow = cursor.GetCurrentWindow();
        assert(cursorWindow);

        cursorWindow->ProcessEvent(cursorWindow, event);
    } else {
        m_currentWindow->ProcessEvent(m_currentWindow, event);
    }
}

void EventManager::Stop()
{
    std::cout << "Event loop was stopped!";
    stopLoop = true;
}
