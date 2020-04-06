//
// Created by romaonishuk on 01.12.19.
//

#include <omp.h>
#include <cstdint>

#ifndef LEXI_TYPES_HPP
#define LEXI_TYPES_HPP

using width_t = uint32_t;
using height_t = uint32_t;

enum Color
{
    kBlack = 0x0,
    kGray = 0xC0C0C0,
    kWhite = 0xFFFFFF,
    kBlue = 0x0000FF,
    kLightBlue = 0xD6EBFF
};

enum class EventType
{
    KeyPressed,
    ButtonPressed,
    ButtonReleased,
    FocusedIn
};

enum class Alignment
{
    kLeft,
    kCenter,
    kRight
};

struct Point
{
    uint32_t x = 0;
    uint32_t y = 0;

    bool operator==(const Point& rhs) { return x == rhs.x && y == rhs.y; }
};

struct GlyphParams
{
    uint32_t x = 0;
    uint32_t y = 0;
    width_t width = 0;
    height_t height = 0;

    bool IsIntersects(const Point& p)
    {
        if(p.x >= x && p.x <= x + width) {
            if(p.y >= y && p.y <= y + height) {
                return true;
            }
        }
        return false;
    }

    operator Point() const { return {x, y}; }
};

#endif  // LEXI_TYPES_HPP
