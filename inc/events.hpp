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
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMotion,
    Scroll
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
    [[nodiscard]] bool IsString() const
    {
        return m_key >= static_cast<uint32_t>(Key::kSpace) && m_key <= static_cast<uint32_t>(Key::kTilde);
    }
    [[nodiscard]] Key GetKey() const { return static_cast<Key>(m_key); }

    uint32_t m_key;
};

class MouseButtonPressedEvent: public Event
{
public:
    enum class MouseButton
    {
        kLeft,
        kMiddle,
        kRight
    };

    MouseButtonPressedEvent(const Point& p, const MouseButton& button):
        Event(p, EventType::MouseButtonPressed),
        m_button(button)
    {}

    [[nodiscard]] MouseButton GetButton() const { return m_button; }

private:
    MouseButton m_button;
};

class ScrollEvent: public Event
{
public:
    enum class Direction
    {
        kUp,
        kDown
    };

    ScrollEvent(const Point& p, Direction direction): Event(p, EventType::Scroll), m_direction(direction) {}

    [[nodiscard]] height_t GetScrolledDistance() const { return scrollDistance; }
    [[nodiscard]] Direction GetDirection() const { return m_direction; }

private:
    // TODO(rmn): static
    const height_t scrollDistance = 15;  // px
    Direction m_direction;
};

}  // namespace Lexi

#endif  // LEXI_I_EVENT_HPP
