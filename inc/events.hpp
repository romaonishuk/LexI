//
// Created by romaonishuk on 04.03.20.
//

#ifndef LEXI_I_EVENT_HPP
#define LEXI_I_EVENT_HPP

#include "types.hpp"

namespace Lexi {
enum class EventType
{
    kNone = 0,
    KeyPressed,
    ButtonPressed,
    ButtonReleased,
    FocusedIn
};

class Event
{
public:
    Event(const Point& p, const EventType& ev): m_eventType(ev), m_point(p) {}
    virtual ~Event() = default;

    const Point& GetPoint() const { return m_point; }
    const EventType& GetEvent() const { return m_eventType; }

private:
    EventType m_eventType = EventType::kNone;
    Point m_point;
};

class KeyBoardEvent: public Event
{
public:
    KeyBoardEvent(const Point&p, const std::string& key):Event(p, EventType::KeyPressed), m_key(key){

    }
    std::string m_key;
};
}  // namespace Lexi

#endif  // LEXI_I_EVENT_HPP
