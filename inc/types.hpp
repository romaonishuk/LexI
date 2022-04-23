//
// Created by romaonishuk on 01.12.19.
//

#include <omp.h>
#include <cstdint>
#include <ostream>

#ifndef LEXI_TYPES_HPP
#define LEXI_TYPES_HPP

using width_t = int32_t;
using height_t = int32_t;

enum Color
{
    kBlack = 0x0,
    kGray = 0xC0C0C0,
    kLightGray = 0xD3D3D3,
    kWhite = 0xFFFFFF,
    kBlue = 0x0000FF,
    kLightBlue = 0xD6EBFF
};

enum class Alignment
{
    kLeft,
    kCenter,
    kRight
};

struct Point
{
    int32_t x = 0;
    int32_t y = 0;

    Point() = default;
    Point(int32_t _x, int32_t _y): x(_x), y(_y) {}

    bool operator==(const Point& rhs) { return x == rhs.x && y == rhs.y; }

    friend std::ostream& operator<<(std::ostream& os, const Point& dt);
};

inline std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "x: " << point.x << " y: " << point.y ;
    return os;
}

struct GlyphParams
{
    int32_t x = 0;
    int32_t y = 0;
    width_t width = 0;
    height_t height = 0;

    GlyphParams() = default;
    GlyphParams(int32_t _x, int32_t _y, width_t w, height_t h): x(_x), y(_y), width(w), height(h) {}
    GlyphParams(const Point&p, width_t w, height_t h): x(p.x), y(p.y), width(w), height(h) {}

    bool IsIntersects(const Point& p) const noexcept
    {
        if(p.x >= x && p.x <= x + width) {
            if(p.y >= y && p.y <= y + height) {
                return true;
            }
        }
        return false;
    }

    bool IsIntersects(const GlyphParams& params) const
    {
        return IsIntersects({params.x, params.y}) || IsIntersects({params.x + params.width, params.y}) ||
            IsIntersects({params.x, params.y + params.height}) ||
            IsIntersects({params.x + params.width, params.y + params.height});
    }

    operator Point() const { return {x, y}; }

    friend std::ostream& operator<<(std::ostream& os, const GlyphParams& params)
    {
        os << "X: " << params.x << " Y: " << params.y << " W: " << params.width << " H: " << params.height;
        return os;
    }
};

#endif  // LEXI_TYPES_HPP
