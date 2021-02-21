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

class EventManager
{
public:
    void RunLoop();
    void Stop();
    void addWindow(Gui::Window* w) { childWindows.push_back(w); }
    void setMainWindow(Gui::Window* window){m_mainWindow = window; m_currentWindow = m_mainWindow;}

    static EventManager& Get();

    // TODO(rmn): make this protected after EventManagerImpl implemented
    [[nodiscard]] Gui::Window* GetCurrentWindow() const {
        return m_currentWindow;
    }

private:
    explicit EventManager();
    ~EventManager() = default;

    void ProcessCursorRelatedEvent(const Lexi::Event&);

    //    std::queue<std::unique_ptr<IEvent>> m_queue;
    std::vector<Gui::Window*> childWindows;
    Gui::Window* m_mainWindow = nullptr;
    Gui::Window* m_currentWindow = nullptr;
    bool stopLoop = false;

    bool ChangeCurrentWindow(unsigned long window);

//    struct EventManagerImpl;
//    std::unique_ptr<EventManagerImpl> m_impl;
};

#endif  // LEXI_EVENT_MANAGER_HPP
