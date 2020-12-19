//
// Created by romaonishuk on 29.10.19.
//

#ifndef LEXI_EVENT_MANAGER_HPP
#define LEXI_EVENT_MANAGER_HPP

#include <cassert>
#include <chrono>
#include <memory>
#include <queue>
#include <thread>

#include "events.hpp"

namespace Gui {
class Window;
}

// TODO(rmn): make singleton
class EventManager
{
public:
    explicit EventManager();
    void RunLoop();
    void Stop();
    void addWindow(Gui::Window* w) { childWindows.push_back(w); }
    void setMainWindow(Gui::Window* window){m_mainWindow = window;}

private:
    void ProcessCursorRelatedEvent(const Lexi::Event&);

    //    std::queue<std::unique_ptr<IEvent>> m_queue;
    std::vector<Gui::Window*> childWindows;
    Gui::Window* m_mainWindow = nullptr;
    Gui::Window* m_currentWindow = nullptr;
    bool stopLoop = false;

    bool ChangeCurrentWindow(unsigned long window);
};

#endif  // LEXI_EVENT_MANAGER_HPP
