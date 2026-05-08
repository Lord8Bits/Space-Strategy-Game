#pragma once
#include <cstdlib>

struct Vec2{
    int x;
    int y;

    int distanceTo(const Vec2& other) const{
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    bool operator==(const Vec2& other) const{
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2& other) const{
        return !(*this == other);
    }
};