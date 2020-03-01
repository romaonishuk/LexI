//
// Created by romaonishuk on 01.12.19.
//

#include <stdint.h>

#ifndef LEXI_TYPES_HPP
#define LEXI_TYPES_HPP

using width_t = uint32_t;
using height_t = uint32_t;

enum Color
{
    kBlack = 0x0,
    kGray = 0xC0C0C0,
    kWhite = 0xFFFFFF,
};

enum class EventType
{
    KeyPressed,
    ButtonPressed,
    ButtonReleased
};

struct Point
{
    uint32_t x = 0;
    uint32_t y = 0;

    bool operator==(const Point& rhs) {
        return x == rhs.x && y == rhs.y;
    }
};

struct GlyphParams
{
    uint32_t x = 0;
    uint32_t y = 0;
    width_t width = 0;
    height_t height = 0;

    bool IsIntersects(const Point &p) {
        if(p.x >= x && p.x <= x+width){
            if(p.y >= y && p.y <= y + height) {
                return true;
            }
        }
        return false;
    }
};

#endif //LEXI_TYPES_HPP
