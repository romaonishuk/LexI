//
// Created by romaonishuk on 24.02.20.
//

#include <X11/Xlib.h>

#include "event_manager.hpp"

#include "window.hpp"
#include "lexi_linux/inc/x_window_impl.hpp"

#include <iostream>

EventManager::EventManager(Gui::Window* w):m_window(w) {

}

void EventManager::RunLoop()
{
    // TODO(rmn): redesign this
    auto* d = m_window->m_window_impl.get();
    auto* display = dynamic_cast<Gui::XWindowImpl*>(d)->m_display;
    while(true)
    {
        XEvent event;
        XNextEvent(display, &event);

        Point p{static_cast<uint32_t>(event.xbutton.x), static_cast<uint32_t>(event.xbutton.y)};
        switch(event.type) {
            case Expose:
                m_window->Draw(m_window);
                break;
            case DestroyNotify:
                std::cout << "Close received!" << std::endl;
                return;
            case ButtonPress:
                std::cout << "ButtonPress" << event.xbutton.x << event.xbutton.y << std::endl;
                m_window->ProcessEvent(m_window, p, EventType::ButtonPressed);
                break;
            case ButtonRelease:
                std::cout << "ButtonRelease" << std::endl;
                m_window->ProcessEvent(m_window, p, EventType::ButtonReleased);
                break;
            case ClientMessage:
                // TODO(rmn): learn about client messages!
                std::cout << "Close received!" << std::endl;
                return ;
            default:
                std::cout << "RMN unprocessed event:" << event.type << std::endl;
                break;
        }
    }
}