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

enum class Key
{
    kSpace = 0x0020,
    kTilde = 0x007e,
    kEnter = 0xff0d,

    kBackspace = 0xff08,
    kArrowLeft = 0xff51,
    kArrowUp = 0xff52,
    kArrowRight = 0xff53,
    kArrowDown = 0xff54,

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
    KeyBoardEvent(const Point& p, uint32_t key): Event(p, EventType::KeyPressed), m_key(key) {}
    KeyBoardEvent(const Point& p, Key key): Event(p, EventType::KeyPressed), m_key(static_cast<uint32_t>(key)) {}
    bool IsString() const
    {
        return m_key >= static_cast<uint32_t>(Key::kSpace) && m_key <= static_cast<uint32_t>(Key::kTilde);
    }
    [[nodiscard]] Key GetKey() const { return static_cast<Key>(m_key); }

    uint32_t m_key;
};
}  // namespace Lexi

#endif  // LEXI_I_EVENT_HPP
