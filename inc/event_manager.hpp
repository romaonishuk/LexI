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

// TODO(rmn): try tio use smtp
class IEvent
{
private:
// EventType mType;
};

class EventManager
{
public:
    explicit EventManager(Gui::Window* window);
    void RunLoop();

private:
//    std::queue<std::unique_ptr<IEvent>> m_queue;
    Gui::Window *m_window;
};

#endif //LEXI_EVENT_MANAGER_HPP
