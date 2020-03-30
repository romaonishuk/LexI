//
// Created by romaonishuk on 29.10.19.
//

#ifndef LEXI_EVENT_MANAGER_HPP
#define LEXI_EVENT_MANAGER_HPP

#include <cassert>
#include <memory>
#include <queue>
#include <chrono>
#include <thread>

namespace Gui {
class Window;
}


// TODO(rmn): make singleton
class EventManager
{
public:
    explicit EventManager(Gui::Window* window);
    void RunLoop();
    void Stop();
    void addWindow(Gui::Window* w) {
        childWindows.push_back(w);
    }
private:
//    std::queue<std::unique_ptr<IEvent>> m_queue;
    std::vector<Gui::Window*> childWindows;
    Gui::Window *m_mainWindow;
    Gui::Window *m_currentWindow;
    bool stopLoop = false;

    bool ChangeCurrentWindow(unsigned long window);
};

#endif //LEXI_EVENT_MANAGER_HPP
